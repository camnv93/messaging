#pragma once
#include <cstdint>
#include <stdio.h>
#include <vector>

namespace msghub::net {
  class Utils {
    public:
    static ssize_t RecvNonBlocking(int32_t fd, uint8_t* buffer, size_t buffer_size);
    static ssize_t ReadAll(int32_t fd, std::vector<uint8_t>& out);
   public:
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;
    Utils(Utils&&) = delete;
    Utils& operator=(Utils&&) = delete;
    Utils() = delete;
  };
}  // namespace msghub::net