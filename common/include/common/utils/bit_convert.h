#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace msghub::common {
  class bit {
   public:
    static uint8_t GetUInt8(const uint8_t* data) noexcept;
    static uint16_t GetUInt16(const uint8_t* data) noexcept;
    static uint32_t GetUInt32(const uint8_t* data) noexcept;
    static void PutUInt8(uint8_t* data, uint8_t value) noexcept;
    static void PutUInt16(uint8_t* data, uint16_t value) noexcept;
    static void PutUInt32(uint8_t* data, uint32_t value) noexcept;
    static std::vector<uint8_t> ToNetWorkBytes(uint8_t value) noexcept;
    static std::vector<uint8_t> ToNetWorkBytes(uint16_t value) noexcept;
    static std::vector<uint8_t> ToNetWorkBytes(uint32_t value) noexcept;
   public:
    bit(const bit&) = delete;
    bit& operator=(const bit&) = delete;
    bit(bit&&) = delete;
    bit& operator=(bit&&) = delete;
    bit() = delete;
  };
}  // namespace msghub::common