// tlv_viewer.h
#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "common/codec/tlv/tlv.h"
#include "common/utils/error.h"

namespace msghub::common {
  class TlvReaderImpl;
  class TlvReader;
  class TlvView {
   public:
    TlvType Type() const { return type_; }
    TlvLength Length() const { return length_; }
    const uint8_t* Value() const { return value_; }

    const uint8_t* Data() const { return value_; }
    size_t Size() const { return length_; }

    bool IsValid() const { return (value_ != nullptr || length_ == 0); }

    Error AsUInt8(uint8_t& value) const;
    Error AsUInt16(uint16_t& value) const;
    Error AsUInt32(uint32_t& value) const;
    Error AsString(std::string& value) const;
    Error AsBytes(std::vector<uint8_t>& value) const;

   private:
    friend class TlvReader;
    friend class TlvReaderImpl;

    TlvView() = default;
    TlvView(TlvType type, TlvLength length, const uint8_t* value)
        : type_(type), length_(length), value_(value) {}

   private:
    TlvType type_{0};
    TlvLength length_{0};
    const uint8_t* value_{nullptr};
  };
}  // namespace msghub::common