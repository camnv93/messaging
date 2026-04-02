#pragma once
#include <stdlib.h>
#include <cstdint>
#include <memory>
#include <string>
#include "common/common.h"
#include "core/iconnection.h"

namespace msghub::core {
  class TcpConnection : public IConnection {
   public:
    TcpConnection(std::string host, int32_t port);
    ~TcpConnection();

    bool Open() override;
    void Close() override;

    int Send(const uint8_t* data, size_t len) override;
    int Receive(uint8_t* buf, size_t max_len) override;
    bool IsConnected() const override;
    int GetFd() const override;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::core