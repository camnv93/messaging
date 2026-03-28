#pragma once
#include <cstdint>
//
// ==============================
// TLV FORMAT DEFINITION
// ==============================
// [type: uint16][length: uint32][value: bytes]
// [TYPE:   uint16_t]  (2 bytes)
// [LENGTH: uint32_t]  (4 bytes)
// [VALUE:  bytes   ]  (N bytes)
//
// Total header size = 6 bytes
//

namespace msg::common {

  constexpr std::size_t TYPE_SIZE = sizeof(uint16_t);
  constexpr std::size_t LENGTH_SIZE = sizeof(uint32_t);
  constexpr std::size_t HEADER_SIZE = TYPE_SIZE + LENGTH_SIZE;
  using TlvType = uint16_t;
  using TlvLength = uint32_t;

  struct TlvHeader {
    TlvType type;
    TlvLength length;
  };

  inline std::size_t TotalSize(TlvLength value_len) noexcept {
    return HEADER_SIZE + value_len;
  }

  inline bool IsValidLength(std::size_t remaining, TlvLength len) noexcept {
    return len <= remaining;
  }
}  // namespace msg::common