//tlv_reader.cpp
#include "common/codec/tlv/tlv_reader.h"
#include "common/utils/bit_convert.h"
#include "common/utils/nbo.h"

#include <cstring>
#include <iostream>

namespace msghub::common {
  class TlvReaderImpl {
   public:
    explicit TlvReaderImpl(const uint8_t* buffer, size_t len) noexcept
        : buffer_(buffer), len_(len), next_tlv_index_(0) {}
    ~TlvReaderImpl() { next_tlv_index_ = 0; };

    TlvView Next() {
      if (!buffer_) {
        return {};
      }

      const auto tlv_offset = next_tlv_index_;
      const auto tlv_data_offset = tlv_offset + HEADER_SIZE;
      if (tlv_data_offset >= len_) {
        return {};
      }

      const uint8_t* tlv_buff = buffer_ + tlv_offset;
      if (tlv_buff == nullptr) {
        return {};
      }

      auto type = bit::GetUInt16(tlv_buff);
      auto len = bit::GetUInt16(tlv_buff + TYPE_SIZE);
      if ((tlv_data_offset + len) >= len_) {
        return {};
      }

      return {type, len, buffer_ + tlv_data_offset};
    }

    bool HasNext() const {
      if (!buffer_) {
        return false;
      }
      return (next_tlv_index_ + HEADER_SIZE <= len_);
    }
    const uint8_t* buffer_;
    size_t len_;
    size_t next_tlv_index_;
  };

  TlvReader::TlvReader(const uint8_t* buffer, size_t len) noexcept
      : impl_(std::make_unique<TlvReaderImpl>(buffer, len)) {};

  TlvReader::~TlvReader() = default;

  TlvReader::TlvReader(TlvReader&& other) noexcept = default;
  TlvReader& TlvReader::operator=(TlvReader&& other) noexcept = default;

  TlvView TlvReader::Next() {
    return impl_->Next();
  }

  void TlvReader::Reset() {
    impl_->next_tlv_index_ = 0;
  }
  bool TlvReader::HasNext() const {
    return impl_->HasNext();
  }

}  // namespace msghub::common