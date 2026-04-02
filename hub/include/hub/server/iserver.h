#pragma once
#include <stddef.h>

namespace msghub::hub {

  class IServer {
   public:
    virtual ~IServer() = default;
    virtual void Run() = 0;

    bool IsRunning() const { return this->is_running_; }
    size_t GetConnections() const { return this->connection_; }

   protected:
    IServer() {};
    bool is_running_{false};
    size_t connection_{0};
  };
}  // namespace msghub::hub