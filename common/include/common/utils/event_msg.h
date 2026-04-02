#pragma once
#include <cstdint>
#include <memory>

namespace msghub::common {
  using EventID = uint32_t;
  class EventMsg {
   public:
    EventID what{0};
    int32_t event_1{0};
    int32_t event_2{0};
  };
}  // namespace msghub::common
