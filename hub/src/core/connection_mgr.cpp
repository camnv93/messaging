#include <mutex>
#include <unordered_map>

#include "hub/core/connection_mgr.h"
namespace msghub::hub {
  struct ConnectionMgr::Impl {
    void AddConnection(std::shared_ptr<IConnection> conn) {
      std::lock_guard<std::mutex> lock(mtx_);
      map_conn_[conn->ID()] = conn;
    };
    void RemoveConnection(std::shared_ptr<IConnection> conn) {
      std::lock_guard<std::mutex> lock(mtx_);
      map_conn_.erase(conn->ID());
    };
    std::shared_ptr<IConnection> Get(msghub::common::ServiceId service_id) {
      std::lock_guard<std::mutex> lock(mtx_);
      auto it = map_conn_.find(service_id);

      if (it != map_conn_.end()) {
        return it->second;
      } else {
        return nullptr;
      }
    }
    std::mutex mtx_;
    std::unordered_map<common::ServiceId, std::shared_ptr<IConnection>>
      map_conn_;
  };
  ConnectionMgr::ConnectionMgr() : impl_(std::make_unique<Impl>()) {};
  ConnectionMgr::~ConnectionMgr() = default;
  void ConnectionMgr::AddConnection(std::shared_ptr<IConnection> conn) {
    impl_->AddConnection(conn);
  };
  void ConnectionMgr::RemoveConnection(std::shared_ptr<IConnection> conn) {
    impl_->RemoveConnection(conn);
  };
  std::shared_ptr<IConnection> ConnectionMgr::Get(
    msghub::common::ServiceId service_id) {
    return impl_->Get(service_id);
  }
}  // namespace msghub::hub