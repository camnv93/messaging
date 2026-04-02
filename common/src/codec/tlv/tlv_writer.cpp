// tlv_writer.cpp
#include <memory>
#include <stack>

#include <limits>
#include "common/buffer/byte_buffer.h"
#include "common/codec/tlv/tlv_writer.h"
#include "common/utils/bit_convert.h"
#include "common/utils/nbo.h"

namespace msghub::common {
  struct TlvWriter::Impl {
   public:
    Impl(ByteBuffer& buffer) : buffer_(buffer) {}
    ~Impl() = default;
    Error IsWritable(size_t len) {
      if (len > std::numeric_limits<TlvLength>::max()) {
        return Error::TLV_LengthOverflow;
      }
      if (buffer_.Size() > std::numeric_limits<TlvLength>::max()) {
        return Error::TLV_LengthOverflow;
      }
      if (buffer_.Size() + len > std::numeric_limits<TlvLength>::max()) {
        return Error::TLV_LengthOverflow;
      }
      return Error::Ok;
    }
    Error WriteHeaderAt(size_t index, TlvType type, TlvLength size) {
      auto ret = this->buffer_.WriteUInt16At(index, type);
      if (ret != Error::Ok) {
        return ret;
      }
      ret = this->buffer_.WriteUInt16At(index + 2U, size);
      return ret;
    }

    Error WriteTLV(TlvType type, const uint8_t* bytes, size_t len) {
      if (bytes == nullptr && len != 0) {
        return Error::InvalidArgument;
      }
      if (len > std::numeric_limits<TlvLength>::max()) {
        return Error::TLV_LengthOverflow;
      }
      auto ret = IsWritable(HEADER_SIZE + len);
      if (ret != Error::Ok) {
        return ret;
      }
      size_t start = buffer_.Size();

      ret = buffer_.SkipWrite(HEADER_SIZE);
      if (ret != Error::Ok)
        return ret;

      ret = buffer_.WriteBytes(bytes, len);
      if (ret != Error::Ok) {
        buffer_.Resize(start);
        return ret;
      }
      ret = WriteHeaderAt(start, type, len);
      if (ret != Error::Ok) {
        buffer_.Resize(start);
      }
      return ret;
    }

    Error Begin(TlvType type) {
      auto offset = buffer_.Size();
      auto ret = WriteHeaderAt(offset, type, 0U);
      if (ret != Error::Ok) {
        return ret;
      }
      offset_stack_.push(offset);
      return Error::Ok;
    }
    Error End() {
      if (offset_stack_.empty()) {
        return Error::TLV_InvalidState;
      }

      auto start = offset_stack_.top();
      offset_stack_.pop();
      if (start > buffer_.Size()) {
        return Error::TLV_InvalidState;
      }
      if (HEADER_SIZE > (buffer_.Size() - start)) {
        return Error::TLV_InvalidState;
      }
      size_t len = buffer_.Size() - start - HEADER_SIZE;
      if (len > std::numeric_limits<TlvLength>::max()) {
        return Error::TLV_LengthOverflow;
      }
      return buffer_.WriteUInt16At(start + 2U, static_cast<TlvLength>(len));
    }

    ByteBuffer& buffer_;
    std::stack<size_t> offset_stack_;
  };

  TlvWriter::TlvWriter(ByteBuffer& buffer)
      : impl_(std::make_unique<Impl>(buffer)) {};
  TlvWriter::~TlvWriter() = default;
  TlvWriter::TlvWriter(TlvWriter&& other) noexcept = default;
  TlvWriter& TlvWriter::operator=(TlvWriter&& other) noexcept = default;

  Error TlvWriter::WriteUInt8(TlvType type, uint8_t value) {
    auto bytes = bit::ToNetWorkBytes(value);
    return impl_->WriteTLV(type, bytes.data(), bytes.size());
  }
  Error TlvWriter::WriteUInt16(TlvType type, uint16_t value) {
    auto bytes = bit::ToNetWorkBytes(value);
    return impl_->WriteTLV(type, bytes.data(), bytes.size());
  }
  Error TlvWriter::WriteUInt32(TlvType type, uint32_t value) {
    auto bytes = bit::ToNetWorkBytes(value);
    return impl_->WriteTLV(type, bytes.data(), bytes.size());
  }
  Error TlvWriter::WriteString(TlvType type, const std::string& value) {
    return impl_->WriteTLV(type, reinterpret_cast<const uint8_t*>(value.data()),
                           value.length());
  }
  Error TlvWriter::WriteBytes(TlvType type, const uint8_t* data, size_t len) {
    return impl_->WriteTLV(type, data, len);
  }
  Error TlvWriter::Begin(TlvType type) {
    return impl_->Begin(type);
  }
  Error TlvWriter::End() {
    return impl_->End();
  }
}  // namespace msghub::common