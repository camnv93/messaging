// message.h
//
#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "common/protocol/message_header.h"
#include "common/protocol/payload.h"
#include "common/utils/error.h"

namespace msghub::common {

using PayloadChecksum = uint32_t;

class Message {
public:
    Message(const uint8_t* raw_data, size_t len);
    // Message(const uint8_t* raw_data, size_t len, const MessageHeader& header);
    Message(const Message&) = delete;
    Message& operator=(const Message&) = delete;
    Message(Message&&) noexcept;
    Message& operator=(Message&&) noexcept;
    ~Message();

    Error Parse();
    Error GetError() const;

    MessageHeader GetHeader() const;
    Payload GetPayload() const;
    PayloadChecksum GetPayloadChecksum() const;

    const uint8_t* RawData() const;
    size_t TotalSize() const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace msghub::common