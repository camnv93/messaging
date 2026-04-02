#include "common/codec/tlv/tlv_view.h"
#include "common/utils/bit_convert.h"

namespace msghub::common {
  Error TlvView::AsUInt8(uint8_t& value) const {
    if (!IsValid() || length_ != sizeof(value))
      return Error::TLV_LengthMismatch;
    value = bit::GetUInt8(value_);
    return Error::Ok;
  }
  Error TlvView::AsUInt16(uint16_t& value) const {
    if (!IsValid()) {
      return Error::TLV_InvalidState;
    }
    if (length_ != sizeof(value) || value_ == nullptr) {
      return Error::TLV_InvalidState;
    }
    value = bit::GetUInt16(value_);
    return Error::Ok;
  }
  Error TlvView::AsUInt32(uint32_t& value) const {
    if (!IsValid()) {
      return Error::TLV_InvalidType;
    }
    if (length_ != sizeof(value) || value_ == nullptr) {
      return Error::TLV_InvalidState;
    }
    value = bit::GetUInt32(value_);
    return Error::Ok;
  }
  Error TlvView::AsString(std::string& value) const {
    if (!IsValid()) {
      return Error::TLV_InvalidState;
    }
    if (value_ == nullptr || length_ == 0) {
      value.clear();
      return Error::Ok;
    }
    value.assign(value_, value_ + length_);
    return Error::Ok;
  }
  Error TlvView::AsBytes(std::vector<uint8_t>& value) const {
    if (!IsValid()) {
      return Error::TLV_InvalidType;
    }
    if (value_ == nullptr || length_ == 0) {
      value.clear();
      return Error::Ok;
    }
    value.assign(value_, value_ + length_);
    return Error::Ok;
  }
}  // namespace msghub::common