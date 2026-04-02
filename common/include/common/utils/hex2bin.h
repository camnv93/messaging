#pragma once
#include <cstdint>
#include <string>
namespace msghub::common {
  class hex2bin {
   public:
    static std::string Bin2Hex(const uint8_t* data, size_t len);
   public:
    hex2bin(const hex2bin&) = delete;
    hex2bin& operator=(const hex2bin&) = delete;
    hex2bin(hex2bin&&) = delete;
    hex2bin& operator=(hex2bin&&) = delete;
    hex2bin() = delete;
  };
}  // namespace msghub::common