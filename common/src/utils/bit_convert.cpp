#pragma once
#include "common/utils/bit_convert.h"
#include <cassert>
#include <cstring>

#include "common/utils/nbo.h"

namespace msghub::common {
  uint8_t bit::GetUInt8(const uint8_t* data) noexcept {
    assert(data != nullptr && "data NULL");
    uint8_t val{0};
    std::memcpy(&val, data, sizeof(uint8_t));
    return val;
  }
  uint16_t bit::GetUInt16(const uint8_t* data) noexcept {
    assert(data != nullptr && "data NULL");
    uint16_t val{0};
    std::memcpy(&val, data, sizeof(uint16_t));
    return nbo::to_host_u16(val);
  }
  uint32_t bit::GetUInt32(const uint8_t* data) noexcept {
    assert(data != nullptr && "data NULL");
    uint32_t val{0};
    std::memcpy(&val, data, sizeof(uint32_t));
    return nbo::to_host_u32(val);
  }

  void bit::PutUInt8(uint8_t* data, uint8_t value) noexcept {
    assert(data != nullptr && "data NULL");
    std::memcpy(data, &value, sizeof(uint8_t));
  }
  void bit::PutUInt16(uint8_t* data, uint16_t value) noexcept {
    assert(data != nullptr && "data NULL");
    uint16_t net_val = nbo::to_network_u16(value);
    std::memcpy(data, &net_val, sizeof(uint16_t));
  }
  void bit::PutUInt32(uint8_t* data, uint32_t value) noexcept {
    assert(data != nullptr && "data NULL");
    uint32_t net_val = nbo::to_network_u32(value);
    std::memcpy(data, &net_val, sizeof(uint32_t));
  }

  std::vector<uint8_t> bit::ToNetWorkBytes(uint8_t value) noexcept {
    std::vector<uint8_t> bytes{value};
    return bytes;
  }
  std::vector<uint8_t> bit::ToNetWorkBytes(uint16_t value) noexcept {
    std::vector<uint8_t> bytes(2);
    uint16_t net_val = nbo::to_network_u16(value);
    std::memcpy(bytes.data(), &net_val, sizeof(value));
    return bytes;
  }
  std::vector<uint8_t> bit::ToNetWorkBytes(uint32_t value) noexcept {
    std::vector<uint8_t> bytes(4);
    uint32_t net_val = nbo::to_network_u32(value);
    std::memcpy(bytes.data(), &net_val, sizeof(value));
    return bytes;
  }
}  // namespace msghub::common