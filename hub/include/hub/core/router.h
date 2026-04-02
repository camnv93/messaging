#pragma once
#include <memory>
#include "iconnection.h"
using namespace msghub::common;
namespace msghub::core {
  class Router {
   public:
    Router();
    ~Router();

    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;
    Router(Router&&) = delete;
    Router& operator=(Router&&) = delete;

    void SubscribeEvent(ServiceId service_id, EventType event_type = 0,
                        std::shared_ptr<IConnection> conn);
    void UnsubscribeEvent(ServiceId service_id, EventType event_type = 0,
                          std::shared_ptr<IConnection> conn);

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::core
