#include <iostream>
#include <memory>
#include <string>
#include "net/tcp/itcp_event_handler.h"
#include "net/tcp/server_socket.h"
#include "net/tcp/socket.h"
#include "net/tcp/utils.h"
#include "common/utils/looper.h"
using namespace msghub::net;

// class test : public msghub::net::TcpEventHandler {
//  public:
//   void OnReceived(const uint8_t* data, size_t len) override {
//     std::cout << data[0] << std::endl;
//   }
//   void OnError() override {}
//   void OnClose() override {}
// };

class Handler : public ServerEventHandler {
 public:
  Handler() {
    std::cout << "Handler" << std::endl;
    std::shared_ptr<Event> ev = std::make_shared<Event>();
    looper_ = std::make_shared<msghub::common::Looper>(ev.get());
    looper_->Start();
    std::cout << "Handler Done" << std::endl;
  }
  void OnReceived(const uint8_t* data, size_t len) override {
    msghub::common::EventMsg msg;
    msg.event_1 = len;
    looper_->SendEvent(msg);
  }
  void OnNewConnection(int fd) override {}
  void OnClose(int fd) override {}

  class Event : public msghub::common::EvHandler {
   public:
    void EvHandling(const msghub::common::EventMsg& msg) override {
      std::cout << "OnEvent: " << msg.event_1 << std::endl;
    }
  };
  std::shared_ptr<msghub::common::Looper> looper_;
};
int main() {
  auto handler = std::make_shared<Handler>();
  ServerSocket server_socket;
  server_socket.RegisterEventHandler(handler);
  server_socket.Start(8080);
  return 0;
}