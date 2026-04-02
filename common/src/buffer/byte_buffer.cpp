//
// Created by devcpp on 3/27/26.
// byte_buffer.cpp
//

#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <limits>
#include "common/buffer/byte_buffer.h"
#include "common/utils/bit_convert.h"
#include "common/utils/nbo.h"

namespace msghub::common {
  struct ByteBuffer::Impl {
   public:
    Impl() : reader_index_(0) {};
    ~Impl() = default;

   public:
    uint8_t* AppendZeroedUnsafe(size_t len) {
      size_t old = buff_.size();
      if (len > (std::numeric_limits<size_t>::max() - old)) {
        return nullptr;
      }
      buff_.resize(old + len);
      return buff_.data() + old;
    }

    Error SkipWrite(size_t len) {
      size_t old = buff_.size();
      if (len > (std::numeric_limits<size_t>::max() - old)) {
        return Error::Buffer_Overflow;
      }
      buff_.resize(old + len);
      return Error::Ok;
    }

    Error WriteBytes(const uint8_t* data, size_t len) {
      if (!data && (len > 0))
        return Error::InvalidArgument;
      if (len == 0)
        return Error::Ok;

      auto* old_data = this->AppendZeroedUnsafe(len);
      if (old_data == nullptr) {
        return Error::Buffer_Overflow;
      }
      std::memcpy(old_data, data, len);
      return Error::Ok;
    }

    Error WriteStr(const char* str, size_t len) {
      return WriteBytes(reinterpret_cast<const uint8_t*>(str), len);
    }

    Error WriteBytesAt(size_t offset, const uint8_t* data, size_t len) {
      if (!data && (len > 0))
        return Error::InvalidArgument;
      if (len == 0)
        return Error::Ok;
      if (len > std::numeric_limits<size_t>::max() - offset) {
        return Error::InvalidArgument;
      }
      size_t required_capacity = offset + len;
      if (required_capacity > buff_.size()) {
        buff_.resize(required_capacity);
      }
      std::memcpy(buff_.data() + offset, data, len);
      return Error::Ok;
    }
    size_t ReadableBytes() const {
      assert(reader_index_ <= buff_.size());
      return buff_.size() - reader_index_;
    }

    Error ReadUInt8(uint8_t& value) {
      if (ReadableBytes() < 1) {
        return Error::Buffer_Underflow;
      }
      value = buff_[reader_index_];
      reader_index_ += 1;
      return Error::Ok;
    }
    Error ReadUInt16(uint16_t& value) {
      if (ReadableBytes() < 2) {
        return Error::Buffer_Underflow;
      }
      value = bit::GetUInt16(buff_.data() + reader_index_);
      reader_index_ += 2;
      return Error::Ok;
    }

    Error ReadUInt32(uint32_t& value) {
      if (ReadableBytes() < 4) {
        return Error::Buffer_Underflow;
      }
      value = bit::GetUInt32(buff_.data() + reader_index_);
      reader_index_ += 4;
      return Error::Ok;
    }
    Error ReadBytes(uint8_t* dest, size_t len) {
      if (ReadableBytes() < len) {
        return Error::Buffer_Underflow;
      }
      if (!dest && len > 0) {
        return Error::InvalidArgument;
      }
      std::memcpy(dest, buff_.data() + reader_index_, len);
      reader_index_ += len;
      return Error::Ok;
    }

   public:
    std::vector<uint8_t> buff_;
    size_t reader_index_;
  };

  ByteBuffer::ByteBuffer() : impl_(std::make_unique<Impl>()) {}
  ByteBuffer::ByteBuffer(size_t initial_capacity)
      : impl_(std::make_unique<Impl>()) {
    impl_->buff_.reserve(initial_capacity);
  }
  ByteBuffer::~ByteBuffer() = default;

  ByteBuffer::ByteBuffer(ByteBuffer&& other) noexcept = default;

  ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other) noexcept = default;

  Error ByteBuffer::WriteBytes(const uint8_t* data, size_t len) {
    return impl_->WriteBytes(data, len);
  }

  Error ByteBuffer::WriteStr(const char* str, size_t len) {
    return impl_->WriteStr(str, len);
  }

  Error ByteBuffer::WriteBytesAt(size_t offset, const uint8_t* data,
                                 size_t len) {
    return impl_->WriteBytesAt(offset, data, len);
  }
  Error ByteBuffer::WriteUInt8At(size_t offset, uint8_t value) {
    return impl_->WriteBytesAt(offset, reinterpret_cast<const uint8_t*>(&value),
                               sizeof(value));
  }
  Error ByteBuffer::WriteUInt16At(size_t offset, uint16_t value) {
    uint16_t net = nbo::to_network_u16(value);
    return impl_->WriteBytesAt(offset, reinterpret_cast<const uint8_t*>(&net),
                               sizeof(net));
  }
  Error ByteBuffer::WriteUInt32At(size_t offset, uint32_t value) {
    uint32_t net = nbo::to_network_u32(value);
    return impl_->WriteBytesAt(offset, reinterpret_cast<const uint8_t*>(&net),
                               sizeof(net));
  }

  Error ByteBuffer::WriteUInt8(uint8_t value) {
    return impl_->WriteBytes(reinterpret_cast<const uint8_t*>(&value), 1U);
  }

  Error ByteBuffer::WriteUInt16(uint16_t value) {
    uint16_t net = nbo::to_network_u16(value);
    return impl_->WriteBytes(reinterpret_cast<const uint8_t*>(&net),
                             sizeof(net));
  }

  Error ByteBuffer::WriteUInt32(uint32_t value) {
    uint32_t net = nbo::to_network_u32(value);
    return impl_->WriteBytes(reinterpret_cast<const uint8_t*>(&net),
                             sizeof(net));
  }
  const uint8_t* ByteBuffer::Data() const noexcept {
    return impl_->buff_.data();
  }

  size_t ByteBuffer::Size() const noexcept {
    return impl_->buff_.size();
  }

  bool ByteBuffer::Empty() const noexcept {
    return impl_->buff_.empty();
  }

  void ByteBuffer::Reserve(size_t capacity) {
    impl_->buff_.reserve(capacity);
  }

  void ByteBuffer::Clear() {
    impl_->buff_.clear();
    impl_->reader_index_ = 0;
  }

  Error ByteBuffer::ReadUInt8(uint8_t& value) {
    return impl_->ReadUInt8(value);
  }
  Error ByteBuffer::ReadUInt16(uint16_t& value) {
    return impl_->ReadUInt16(value);
  }
  Error ByteBuffer::ReadUInt32(uint32_t& value) {
    return impl_->ReadUInt32(value);
  }

  Error ByteBuffer::SkipBytes(size_t len) {
    if (impl_->reader_index_ > impl_->buff_.size()) {
      return Error::Corruption;
    }
    if (len > (impl_->buff_.size() - impl_->reader_index_)) {
      return Error::Buffer_Underflow;
    }
    impl_->reader_index_ += len;
    return Error::Ok;
  }
  size_t ByteBuffer::ReadableBytes() const {
    return impl_->ReadableBytes();
  }
  void ByteBuffer::ResetReader() {
    impl_->reader_index_ = 0;
  }
  Error ByteBuffer::ReadBytes(uint8_t* dest, size_t len) {
    return impl_->ReadBytes(dest, len);
  }
  Error ByteBuffer::SkipWrite(size_t len) {
    return impl_->SkipWrite(len);
  }
  void ByteBuffer::Resize(size_t size) {
    impl_->buff_.resize(size);
  }
}  // namespace msghub::common
