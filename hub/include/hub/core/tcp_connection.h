#pragma once
#include "iconnection.h"
namespace msghub::hub {
  class TcpConnection : public IConnection {
   public:
    TcpConnection(int32_t fd);
    ~TcpConnection();

    void Close() override;
    bool IsConnected() const override;
    int32_t GetFd() const override;

   private:
    int32_t fd_;
  };
}  // namespace msghub::hub