#include "router.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include "common/common.h"
#include "iconnection.h"

namespace msghub::core {
  struct Router::Impl {
   public:
    Impl() = default;
    ~Impl() = default;
    void SubscribeEvent(ServiceId service_id, EventType event_type,
                        std::shared_ptr<IConnection> conn) {
      uint64_t key = static_cast<uint64_t>(service_id) << 32 | event_type;
      auto& clients = routing_table_[key];
      auto it = std::find_if(clients.begin(), clients.end(),
                             [conn](const std::shared_ptr<IConnection>& c) {
                               return c->GetServiceId() == conn->GetServiceId();
                             });
      if (it == clients.end()) {
        clients.push_back(conn);
      }
    }
    void UnsubscribeEvent(ServiceId service_id, EventType event_type,
                          std::shared_ptr<IConnection> conn) {
      uint64_t key = static_cast<uint64_t>(service_id) << 32 | event_type;
      auto it = routing_table_.find(key);
      if (it != routing_table_.end()) {
        auto& clients = it->second;
        clients.erase(
          std::remove_if(clients.begin(), clients.end(),
                         [conn](const std::shared_ptr<IConnection>& c) {
                           return c->GetServiceId() == conn->GetServiceId();
                         }),
          clients.end());
        if (clients.empty()) {
          routing_table_.erase(it);
        }
      }
    }

   public:
    using ConnectionList = std::vector<std::shared_ptr<IConnection>>;
    std::unordered_map<uint64_t, ConnectionList> routing_table_;
  };

  Router::Router() : impl_(std::make_unique<Impl>()) {}
  Router::~Router() = default;
  Router::Router(Router&&) = default;
  Router& Router::operator=(Router&&) = default;
  void Router::SubscribeEvent(ServiceId service_id, EventType event_type,
                              std::shared_ptr<IConnection> conn) {
    impl_->SubscribeEvent(service_id, event_type, conn);
  }
  void Router::UnsubscribeEvent(ServiceId service_id, EventType event_type,
                                std::shared_ptr<IConnection> conn) {
    impl_->UnsubscribeEvent(service_id, event_type, conn);
  }
}  // namespace msghub::core