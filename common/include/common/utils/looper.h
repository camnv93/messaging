#pragma once
#include <cstdint>
#include <memory>
#ifndef LOG_TAG2
#define LOG_TAG2 "LOOPER"
#endif
#include "common/utils/event_msg.h"
#include "common/utils/logger.h"

namespace msghub::common {
  class IEventHandler {
   public:
    virtual ~IEventHandler() = default;
    virtual void Handle(const EventMsg&) = 0;
  };

  class Looper {
   public:
    Looper(std::shared_ptr<IEventHandler> event_handler, size_t max_event = 1024);
    ~Looper();
    Looper(const Looper& other) = delete;
    Looper& operator=(const Looper& other) = delete;
    Looper(Looper&& other) noexcept = delete;
    Looper& operator=(Looper&& other) noexcept = delete;

    void Start();
    void Shutdown(bool drain = false);
    bool SendEvent(const EventMsg& msg);
    bool SendEventDelay(const EventMsg& msg, uint32_t delay_ms = 0);

    bool HasEvent(EventID event_id);
    bool CancelEvent(EventID event_id);

    size_t QueueSize() const;
   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::common
