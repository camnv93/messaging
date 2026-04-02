#pragma once
#include <cstdint>

namespace msghub::net {
  struct TcpMessage {
    int32_t fd{0};
    std::vector<uint8_t> data;
    TcpMessage(std::vector<uint8_t>&& d, int32_t f = 0)
        : fd(f), data(std::move(d)) {}
  };

  enum class Error : uint32_t {
    Ok = 0,
  };
}  // namespace msghub::net