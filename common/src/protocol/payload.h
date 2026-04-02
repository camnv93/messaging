#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>

#include "common/codec/tlv/tlv_reader.h"
#include "common/utils/error.h"

namespace msghub::common {
  class Payload {
   public:
    Payload() : data_(nullptr), size_(0) {}
    Payload(const uint8_t* data, size_t size) : data_(data), size_(size) {}
    ~Payload() = default;

    Payload(Payload&& other) noexcept = default;
    Payload& operator=(Payload&& other) noexcept = default;
    Payload(const Payload&) = default;
    Payload& operator=(const Payload&) = default;

    TlvReader GetReader() const { return TlvReader(data_, size_); }
    bool Empty() const noexcept { return data_ == nullptr || size_ == 0; }

    const uint8_t* Data() const noexcept { return data_; }
    size_t Size() const noexcept { return size_; }

   private:
    const uint8_t* data_;
    size_t size_;
  };

}  // namespace msghub::common