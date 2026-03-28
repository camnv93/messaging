#pragma once
#include <cstdint>

namespace msg::common {
    enum class MessageType : uint8_t {
        UNKNOWN  = 0,
        REQUEST  = 1,
        RESPONSE = 2,
        EVENT    = 3
    };
    using EventType = uint16_t;
    using ServiceId = uint32_t;

    struct MessageHeader {
        uint8_t     version {1};
        MessageType message_type {MessageType::UNKNOWN};

        ServiceId   source {0};
        ServiceId   destination {0};
        EventType   event_type {0};

        uint64_t    message_id {0};
        uint64_t    correlation_id {0};

        uint32_t    payload_size {0};

        uint16_t    flags {0};
        uint16_t    reserved {0};

        ServiceId   reply_to {0};
    };
 } // namespace msg::common
