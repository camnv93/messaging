#pragma once
#include <cstdint>

namespace msg {
 namespace common {
    enum class MessageType : uint8_t {
        UNKNOWN  = 0,
        REQUEST  = 1,
        RESPONSE = 2,
        EVENT    = 3
    };
    using EventType = uint16_t;
    using ServiceId = uint32_t;

    struct MessageHeader {
        // ===== Basic =====
        uint8_t     version {1};
        MessageType message_type {MessageType::UNKNOWN};

        // ===== Routing (QUAN TRỌNG) =====
        ServiceId   source {0};        // service gửi
        ServiceId   destination {0};   // service / topic nhận
        EventType   event_type {0};    // loại event (KEY để route)

        // ===== Correlation =====
        uint64_t    message_id {0};
        uint64_t    correlation_id {0}; // dùng cho RPC

        // ===== Payload =====
        uint32_t    payload_size {0};

        // ===== Flags / Future =====
        uint16_t    flags {0};
        uint16_t    reserved {0};

        // ===== Optional =====
        ServiceId   reply_to {0};      // dùng cho RPC response
    };
 } // namespace common
} // namespace msg