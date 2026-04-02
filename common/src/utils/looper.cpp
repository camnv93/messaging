#include "common/utils/looper.h"

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_set>
#include <utility>

namespace msghub::common {

  struct Looper::Impl {
   public:
    using Clock = std::chrono::steady_clock;

    using TimePoint = Clock::time_point;

   private:
    enum class State { Stopped, Running, Stopping };

    struct EventNode {
      EventMsg msg;

      TimePoint execute_at;
      uint64_t seq{0};

      bool operator>(const EventNode& other) const { return execute_at > other.execute_at; }
    };

   public:
    explicit Impl(std::shared_ptr<IEventHandler> handler, size_t max_queue_size)
        : handler_(handler),
          max_queue_size_(max_queue_size) {}

    ~Impl() { Shutdown(false); }

   public:
    void Start() {
      std::lock_guard<std::mutex> lock(mtx_);

      if (state_ == State::Running) {
        return;
      }

      state_ = State::Running;

      worker_ = std::thread(&Impl::Loop, this);
    }

    void Shutdown(bool drain) {
      {
        std::lock_guard<std::mutex> lock(mtx_);

        if (state_ != State::Running) {
          return;
        }

        state_ = State::Stopping;

        drain_ = drain;
      }

      cv_.notify_one();

      if (worker_.joinable()) {
        if (std::this_thread::get_id() == worker_.get_id()) {
          std::terminate();
        }

        worker_.join();
      }

      {
        std::lock_guard<std::mutex> lock(mtx_);

        state_ = State::Stopped;

        while (!queue_.empty()) {
          queue_.pop();
        }
        cancelled_.clear();
      }
    }

   public:
    bool PushEvent(const EventMsg& event, uint32_t delay_ms) {
      EventNode node;
      node.msg = event;
      node.seq = ++seq_gen_;
      node.execute_at = Clock::now() + std::chrono::milliseconds(delay_ms);

      {
        std::lock_guard<std::mutex> lock(mtx_);

        if (state_ != State::Running) {
          return false;
        }

        if (queue_.size() >= max_queue_size_) {
          ++dropped_events_;
          return false;
        }

        bool wakeup = queue_.empty() || node.execute_at < queue_.top().execute_at;
        queue_.push(std::move(node));
        if (wakeup) {
          cv_.notify_one();
        }
      }

      return true;
    }

   public:
    bool HasEvent(EventID event_id) {
      std::lock_guard<std::mutex> lock(mtx_);
      std::priority_queue<EventNode, std::vector<EventNode>, std::greater<EventNode>> temp(queue_);
      while (!temp.empty()) {
        auto node = temp.top();
        if(node.msg.what = event_id) {
          return true;
        }
        temp.pop();
      }
      return false;
    }

    bool CancelEvent(EventID event_id) { cancelled_.insert(event_id); }

    bool RemoveEvent(EventID event_id) {}

    size_t QueueSize() {
      std::lock_guard<std::mutex> lock(mtx_);

      return queue_.size();
    }

    uint64_t DroppedEvents() const { return dropped_events_; }

   private:
    void Loop() {
      std::unique_lock<std::mutex> lock(mtx_);

      while (true) {
        if (state_ == State::Stopping) {
          if (!drain_ || queue_.empty()) {
            break;
          }
        }

        if (queue_.empty()) {
          cv_.wait(lock, [&]() { return state_ != State::Running || !queue_.empty(); });

          continue;
        }

        auto now = Clock::now();

        const auto& top = queue_.top();

        if (now < top.execute_at) {
          cv_.wait_until(lock, top.execute_at, [&]() { return state_ != State::Running; });
          continue;
        }

        EventNode node = std::move(const_cast<EventNode&>(queue_.top()));

        queue_.pop();
        if (cancelled_.count(node.msg.what)) {
          cancelled_.erase(node.msg.what);
          continue;
        }

        EventMsg msg = std::move(node.msg);

        lock.unlock();

        Dispatch(msg);

        lock.lock();
      }
    }

   private:
    void Dispatch(const EventMsg& msg) {
      auto handler = handler_.lock();

      if (!handler) {
        return;
      }

      auto start = Clock::now();

      try {
        handler->Handle(msg);
      } catch (...) {
        std::cerr << "Looper handler exception" << std::endl;
      }

      auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count();

      if (cost > 100) {
        std::cerr << "[Looper] slow handler: " << cost << " ms" << std::endl;
      }
    }

   private:
    std::mutex mtx_;

    std::condition_variable cv_;

    std::thread worker_;
    std::unordered_set<EventID> cancelled_;

   private:
    std::weak_ptr<IEventHandler> handler_;

   private:
    State state_{State::Stopped};

    bool drain_{false};

   private:
    std::priority_queue<EventNode, std::vector<EventNode>, std::greater<EventNode>> queue_;

   private:
    std::atomic<uint64_t> seq_gen_{0};
    std::atomic<uint64_t> dropped_events_{0};

   private:
    size_t max_queue_size_;
  };

  Looper::Looper(std::shared_ptr<IEventHandler> handler, size_t max_queue_size)
      : impl_(std::make_unique<Impl>(std::move(handler), max_queue_size)) {}

  Looper::~Looper() = default;

  void Looper::Start() {
    impl_->Start();
  }

  void Looper::Shutdown(bool drain) {
    impl_->Shutdown(drain);
  }

  bool Looper::SendEvent(const EventMsg& msg) {
    return impl_->PushEvent(msg, 0);
  }

  bool Looper::SendEventDelay(const EventMsg& msg, uint32_t delay_ms) {
    return impl_->PushEvent(msg, delay_ms);
  }

  size_t Looper::QueueSize() const {
    return impl_->QueueSize();
  }

}  // namespace msghub::common