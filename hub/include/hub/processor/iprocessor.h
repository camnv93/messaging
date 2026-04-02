#pragma once
#include <cstdint>

#include "common/protocol/message.h"
#include "common/utils/common.h"
#include "hub/core/hub_error.h"

namespace msghub::hub {
  class IProcessorHandler {
   public:
    virtual ~IProcessorHandler() = default;
    virtual void OnReceived(const common::Message& msg) = 0;

   protected:
    IProcessorHandler() = default;
  };

  class IProcessor {
   public:
    virtual ~IProcessor() = default;
    virtual bool Connect() = 0;
    virtual Error Publish(const common::Message& msg) = 0;
    virtual Error Subscribe(common::ServiceId, common::EventType) = 0;
    virtual void Run() = 0;

   protected:
    IProcessor(common::ServiceId serivce_id, std::shared_ptr<IProcessorHandler> handler = nullptr)
        : service_id_(serivce_id),
          handler_(handler) {};

    common::ServiceId service_id_{0};
    std::shared_ptr<IProcessorHandler> handler_{};
  };
}  // namespace msghub::hub