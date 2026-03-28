//
// Created by devcpp on 3/26/26.
//

#pragma once
#include <cstdint>
#include <memory>
#include <vector>

#include "Header.h"

namespace msg::common {
  class Message {
  public:
    Message();
    ~Message();

    Message(Message && other) noexcept;
    Message &operator=(Message && other) noexcept;

    Message(const Message &other);
    Message &operator=(const Message &other);

    void SetMsgType(MessageType message_type);
    MessageType GetMsgType() const;

    void SetSource(ServiceId source);
    ServiceId GetSource() const;

    void SetEvent(EventType event);
    EventType GetEvent() const;

    void SetPayload(const std::vector<uint8_t>& payload);
    void SetPayload(const uint8_t* payload, size_t size);
    std::vector<uint8_t> GetPayload() const;
    size_t PayloadSize() const;

  private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
  };
}
