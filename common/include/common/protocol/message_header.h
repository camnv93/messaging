// message_header.h
//
#pragma once
#include <cstdint>
#include "common/utils/common.h"

namespace msghub::common {
  enum class MessageType : uint8_t {
    UNKNOWN = 0,
    SUBSCRIBE = 1,
    PUBLISHER = 2,
    RESPONSE = 3,
    SEND = 4,
    HEARTBEAT = 5
  };



#pragma pack(push, 1)
  struct MessageHeader {
    uint16_t magic{0xA5A5};  // 0 ->1
    uint8_t version{1};      // 2
    HeaderChecksum header_checksum{0};  // 3

    MessageType message_type{MessageType::UNKNOWN};  // 4
    uint8_t ttl{8};          // 5
    uint16_t flags{0};        // 6 7

    EventType event_type{0};  // 8 9
    ServiceId source{0};       // [10-11]
    ServiceId destination{0};  // [12-13]
    PayloadSize payload_size{0}; // [14-15]
  };
#pragma pack(pop)
}  // namespace msghub::common
