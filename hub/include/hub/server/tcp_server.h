#pragma once
#include <memory>
#include <string>

#include "hub/server/iserver.h"

namespace msghub::hub {

  class TcpServer : public IServer {
   public:
    TcpServer(int port);
    ~TcpServer();
    void Run() override;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::hub