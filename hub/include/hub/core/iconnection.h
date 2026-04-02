#pragma once
#include <stdlib.h>
#include <cstdint>
#include "common/protocol/message_header.h"
namespace msghub::hub {
  class IConnection {
   public:
    IConnection(const IConnection&) = delete;
    IConnection& operator=(const IConnection&) = delete;

    IConnection(IConnection&&) noexcept = default;
    IConnection& operator=(IConnection&&) noexcept = default;

    common::ServiceId ID() const {
      return this->serivce_id_;
    }
    virtual ~IConnection() = default;

    virtual void Close() = 0;
    virtual bool IsConnected() const = 0;
    virtual int GetFd() const = 0;

   protected:
    common::ServiceId serivce_id_;
    IConnection() = default;
  };
}  // namespace msghub::hub