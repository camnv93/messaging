#include "nbo.h"
#include <netinet/in.h>

  namespace msg::common {

    // host -> network (big-endian)
    uint32_t nbo::to_network_u32(uint32_t value) noexcept {
      return htonl(value);
    }

    uint16_t nbo::to_network_u16(uint16_t value) noexcept {
      return htons(value);
    }

    // network -> host
    uint32_t nbo::to_host_u32(uint32_t value) noexcept {
      return ntohl(value);
    }

    uint16_t nbo::to_host_u16(uint16_t value) noexcept {
      return ntohs(value);
    }

  } // namespace msg::common
