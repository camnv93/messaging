//
// Created by devcpp on 3/27/26.
//

#include "byte_buffer.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "utils/nbo.h"

namespace msg::common {
  struct ByteBuffer::Impl {
   public:
    void dump() const {
      std::cout << "############" << std::endl;
      for (const auto& v : buff_) {
        std::cout << std::hex << static_cast<int32_t>(v) << ", ";
      }
      std::cout << "\n############" << std::endl;
    }
    uint8_t* Reserve(size_t len) {
      size_t old = buff_.size();
      buff_.resize(old + len);
      return buff_.data() + old;
    }

    void WriteBytes(const uint8_t* data, size_t len) {
      auto* old_data = this->Reserve(len);
      std::memcpy(old_data, data, len);
    }

    void WriteStr(const char* str, size_t len) {
      WriteBytes(reinterpret_cast<const uint8_t*>(str), len);
    }

    void WriteAt(size_t offset, const uint8_t* data, size_t len) {
      if (data == nullptr && len > 0) {
        //assert(false && "Invalid data");
        return;
      }
      if ((offset + len) > buff_.size()) {
        auto extern_size = (offset + len) - buff_.size();
        size_t old = buff_.size();
        buff_.resize(old + extern_size);
        std::memcpy(buff_.data() + offset, data, len);
      } else {
        std::memcpy(buff_.data() + offset, data, len);
      }
    }

    std::vector<uint8_t> buff_;
  };

  ByteBuffer::ByteBuffer() : impl_(std::make_unique<Impl>()) {}

  ByteBuffer::~ByteBuffer() = default;

  ByteBuffer::ByteBuffer(ByteBuffer&& other) noexcept = default;

  ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other) noexcept = default;

  void ByteBuffer::WriteBytes(const uint8_t* data, size_t len) {
    impl_->WriteBytes(data, len);
  }

  void ByteBuffer::WriteStr(const char* str, size_t len) {
    impl_->WriteStr(str, len);
  }

  void ByteBuffer::WriteAt(size_t offset, const uint8_t* data, size_t len) {
    impl_->WriteAt(offset, data, len);
  }

  void ByteBuffer::WriteUInt8(uint8_t value) {
    impl_->WriteBytes(reinterpret_cast<const uint8_t*>(&value), 1U);
  }

  void ByteBuffer::WriteUInt16(uint16_t value) {
    uint16_t net = nbo::to_network_u16(value);
    impl_->WriteBytes(reinterpret_cast<const uint8_t*>(&net), sizeof(net));
  }

  void ByteBuffer::WriteUInt32(uint32_t value) {
    uint32_t net = nbo::to_network_u32(value);
    impl_->WriteBytes(reinterpret_cast<const uint8_t*>(&net), sizeof(net));
  }

  void ByteBuffer::WriteUInt64(uint64_t value) {
    (void)value;
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
  }

  void ByteBuffer::Dump() const {
    impl_->dump();
  }
}  // namespace msg::common
