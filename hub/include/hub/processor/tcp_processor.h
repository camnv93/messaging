#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "common/utils/logger.h"
#include "iprocessor.h"

namespace msghub::hub {
  class TcpProcessor : public IProcessor {
   public:
    TcpProcessor(const std::string& host,
                 int port,
                 common::ServiceId sv,
                 std::shared_ptr<IProcessorHandler> handler = nullptr,
                 const std::string& log_tag = LOG_TAG);
    ~TcpProcessor();

    bool Connect() override;

    Error Publish(const common::Message& msg) override;
    Error Subscribe(common::ServiceId sv, common::EventType ev) override;

    void Run() override;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::hub