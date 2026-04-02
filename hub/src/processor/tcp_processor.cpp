
#include "hub/processor/tcp_processor.h"

#include <atomic>
#include <iostream>

#include "common/protocol/message.h"
#include "common/protocol/tlv_message_builder.h"
#include "net/tcp/socket.h"

namespace msghub::hub {
  class SocketHandler : public msghub::net::ISocketHandler {
   public:
    SocketHandler(std::shared_ptr<IProcessorHandler> handler)
        : processor_handler_(handler) {};
    ~SocketHandler() = default;

    void OnReceived(msghub::net::TcpMessage&& tcp_msg) override {
      if (processor_handler_) {
        common::Message msg(tcp_msg.data.data(), tcp_msg.data.size());
        processor_handler_->OnReceived(msg);
      }
    }

    void OnError() override {}

    void OnClose() override {}

    std::shared_ptr<IProcessorHandler> processor_handler_;
  };

  struct TcpProcessor::Impl {
   public:
    Impl(std::string host, int port)
        : host_(host),
          port_(port) {}

    ~Impl() = default;

    void Init() {
      socket_handler_ = std::make_shared<SocketHandler>(this->processor_handler_);
      socket_ = std::make_unique<net::Socket>(socket_handler_);
    }

    bool Connect() {
      if (socket_) {
        is_connected_ = socket_->Connect(host_, port_);
      }

      return is_connected_;
    }

    Error Subscribe(common::ServiceId sv, common::EventType ev) {
      if (socket_) {
        common::ByteBuffer buffer(128);
        common::TlvMsgBuilder tvl_builder(buffer);
        auto msg = tvl_builder.SetType(common::MessageType::SUBSCRIBE)
                       .SetSource(service_id_)
                       .SetDestination(sv)
                       .SetEvent(ev)
                       .Build();
        auto ret = socket_->Send(msg.RawData(), msg.TotalSize());
      }
      return Error::Ok;
    }

    Error Publish(const common::Message& msg) { return Error::Ok; }

    void Run() {
      if (is_connected_ && socket_) {
        socket_->Run();
      }
    }

   public:
    common::ServiceId service_id_;
    std::string host_;
    int port_;
    std::atomic<bool> is_connected_{false};
    std::unique_ptr<net::Socket> socket_;
    std::shared_ptr<IProcessorHandler> processor_handler_;
    std::shared_ptr<msghub::net::ISocketHandler> socket_handler_;
  };

  TcpProcessor::TcpProcessor(const std::string& host,
                             int port,
                             common::ServiceId sv,
                             std::shared_ptr<IProcessorHandler> handler,
                             const std::string& log_tag)
      : IProcessor(sv, handler),
        impl_(std::make_unique<Impl>(host, port)) {
    impl_->service_id_ = sv;
    impl_->processor_handler_ = handler;
    impl_->Init();
  }

  TcpProcessor::~TcpProcessor() = default;

  bool TcpProcessor::Connect() {
    return impl_->Connect();
  }

  Error TcpProcessor::Publish(const common::Message& msg) {
    return impl_->Publish(msg);
  }

  Error TcpProcessor::Subscribe(common::ServiceId sv, common::EventType ev) {
    return impl_->Subscribe(sv, ev);
  }

  void TcpProcessor::Run() {
    impl_->Run();
  }
}  // namespace msghub::hub