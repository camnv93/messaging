/*
  Big-endian
  NetWork Byte Order
 */

#pragma once
#include <cstdint>
  namespace msg::common {

    class nbo {
     public:
      static uint32_t to_host_u32(uint32_t value) noexcept;
      static uint16_t to_host_u16(uint16_t value) noexcept;

      static uint32_t to_network_u32(uint32_t value) noexcept;
      static uint16_t to_network_u16(uint16_t value) noexcept;

     public:
      nbo(const nbo&) = delete;
      nbo& operator=(const nbo&) = delete;
      nbo(nbo&&) = delete;
      nbo& operator=(nbo&&) = delete;
      nbo() = delete;
    };

  } // namespace msg::common
