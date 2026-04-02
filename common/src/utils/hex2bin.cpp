
#include "common/utils/hex2bin.h"
#include <iomanip>
#include <sstream>

namespace msghub::common {
  std::string hex2bin::Bin2Hex(const uint8_t* data, size_t len) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
      ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
  }
}  // namespace msghub::common