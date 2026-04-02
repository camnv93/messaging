#include <atomic>
#include <iostream>
#include <random>
#include <thread>

#define LOG_TAG "MAIN"
#include "common/utils/event_msg.h"
#include "common/utils/logger.h"
#include "common/utils/looper.h"
#include "common/utils/time.h"
#include "hub/processor/tcp_processor.h"
#include "hub/server/tcp_server.h"


// class ProcessorHandler : public msghub::hub::IProcessorHandler {
//  public:
//   void OnReceived(const msghub::common::Message& msg) { std::cout << "ProcessorHandler::OnReceived" << std::endl; }
// };
class Mylooper : public msghub::common::IEventHandler {
 public:
  void Handle(const msghub::common::EventMsg& evt) override {
    std::cout << "=====>>>> Handle::evt: " << evt.event_1 << ":" << evt.event_2 << ","
              << msghub::common::Time::GetCurrentTimeMs() << std::endl;
    if (evt.event_1 == 1001 && evt.event_2 < 20) {
      msghub::common::EventMsg evt_new;
      evt_new.event_1 = 1001;
      evt_new.event_2 = evt.event_2 + 1;
      looper_->SendEventDelay(evt_new, 1000);
    }
  }

  msghub::common::Looper* looper_{nullptr};
};

int main() {
  // // std::shared_ptr<msghub::hub::IServer> server = std::make_shared<msghub::hub::TcpServer>(8080);
  // // server->Run();
  // std::shared_ptr<msghub::hub::IProcessor> sv = std::make_shared<msghub::hub::TcpProcessor>(
  //     "host.docker.internal", 9000, 0x1234, std::make_shared<ProcessorHandler>(), LOG_TAG);
  // sv->Connect();
  // sv->Subscribe(0x4321, 0xABCD);
  // sv->Run();
  std::cout << LOG_TAG << std::endl;
  msghub::common::Logger::Init("logz/log.log");
  auto handler_looper = std::make_shared<Mylooper>();
  std::shared_ptr<msghub::common::Looper> looper = std::make_shared<msghub::common::Looper>(handler_looper);
  // handler_looper->looper_ = looper.get();
  looper->Start();
  // msghub::common::EventMsg evt;
  // evt.event_1 = 1003;
  // looper->SendEventDelay(evt, 10000);
  // evt.event_1 = 1001;
  // looper->SendEventDelay(evt, 1000);
  // evt.event_1 = 1000;
  // looper->SendEvent(evt);

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}