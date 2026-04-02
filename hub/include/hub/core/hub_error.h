#pragma once

#include <cstdint>
#include <string>

namespace msghub::hub {
  enum class Error : uint32_t {
    Ok = 0,
    Server_closed,
    Invalid_Msg,
    Unknown = 0xFFFFFFFFu
  };

}  // namespace msghub::hub