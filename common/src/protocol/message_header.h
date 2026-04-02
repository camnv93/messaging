// message_header.h
//
#pragma once
#include <cstdint>

namespace msghub::common {
  enum class MessageType : uint8_t {
    UNKNOWN = 0,
    SUBSCRIBE = 1,
    PUBLISHER = 2,
    RESPONSE = 3,
    SEND = 4,
    HEARTBEAT = 5
  };

  using EventType = uint16_t;
  using ServiceId = uint32_t;
  using HeaderChecksum = uint8_t;

#pragma pack(push, 1)
  struct MessageHeader {
    uint16_t magic{0xA5A5};  // 0 ->1
    uint8_t version{1};      // 2
    HeaderChecksum header_checksum{0};  // 3

    MessageType message_type{0};  // 4
    uint8_t ttl{8};          // 5
    uint16_t flags{0};        // 6 7

    EventType event_type{0};
    ServiceId source{0};       // [12-15]
    ServiceId destination{0};  // [16-19]
    uint32_t payload_size{0}; // [20-23]
  };
#pragma pack(pop)
}  // namespace msghub::common
