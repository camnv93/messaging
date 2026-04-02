#pragma once
#include <cstdint>

namespace msghub::common {
  using EventType = uint16_t;
  using ServiceId = uint16_t;
  using PayloadSize = uint16_t;
  using HeaderChecksum = uint8_t;
  inline constexpr static ServiceId SERVER_ID{0xEFEF};
}  // namespace msghub::common