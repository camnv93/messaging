#pragma once
#include <memory>
#include "iconnection.h"

namespace msghub::hub {
  class ConnectionMgr {
   public:
    ConnectionMgr();
    ~ConnectionMgr();

    ConnectionMgr(const ConnectionMgr& other) = delete;
    ConnectionMgr& operator=(const ConnectionMgr& other) = delete;
    ConnectionMgr(ConnectionMgr&& other) noexcept;
    ConnectionMgr& operator=(ConnectionMgr&& other) noexcept;

    void AddConnection(std::shared_ptr<IConnection> conn);
    void RemoveConnection(std::shared_ptr<IConnection> conn);
    std::shared_ptr<IConnection> Get(msghub::common::ServiceId service_id);
   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::hub