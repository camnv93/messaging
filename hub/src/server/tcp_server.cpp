
#include "hub/server/tcp_server.h"

#include <iostream>
#include <string>

#include "common/utils/looper.h"
#include "hub/core/connection_mgr.h"
#include "net/tcp/server_socket.h"
#include "spdlog/logger.h"

namespace msghub::hub {
  class LoopHandler : public msghub::common::IEventHandler {
   public:
    void Handle(const msghub::common::EventMsg& ev) override {
      std::cout << "LoopHandler::Handle - " << ev.event_1 << std::endl;
    }
  };

  class TcpHandler : public msghub::net::TcpSvHandler {
   public:
    TcpHandler(std::shared_ptr<msghub::common::Looper> loop)
        : looper_(loop) {}

    void OnReceived(net::TcpMessage&& msg) override {
      if (looper_) {
        msghub::common::EventMsg ev;
        ev.event_1 = msg.data.size();
        looper_->SendEvent(ev);
      }
    }

    void OnNewConnection(int fd) override {};
    void OnClose(int fd) override {};
    std::shared_ptr<msghub::common::Looper> looper_;
  };

  struct TcpServer::Impl {
    Impl(int port)
        : port_(port) {}

    void Init() {
      event_handler_ = std::make_shared<LoopHandler>();
      looper_ = std::make_shared<msghub::common::Looper>(event_handler_);
      tcp_handler_ = std::make_shared<TcpHandler>(looper_);
      server_ = std::make_unique<net::ServerSocket>(this->tcp_handler_);
    }

    ~Impl() {}

    void Run() {
      looper_->Start();
      server_->Start(port_);
    }

    std::shared_ptr<msghub::net::TcpSvHandler> tcp_handler_;
    std::shared_ptr<msghub::common::IEventHandler> event_handler_;
    std::shared_ptr<msghub::common::Looper> looper_;
    int port_{};
    std::unique_ptr<net::ServerSocket> server_;
  };

  TcpServer::TcpServer(int port)
      : impl_(std::make_unique<Impl>(port)) {
        impl_->Init();
      };
  TcpServer::~TcpServer() {};

  void TcpServer::Run() {
    impl_->Run();
  }
}  // namespace msghub::hub