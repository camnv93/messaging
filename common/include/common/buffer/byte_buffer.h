//
// Created by devcpp on 3/27/26.
// byte_buffer.h
//

#pragma once
#include <cstdint>
#include <memory>
#include "common/utils/error.h"

namespace msghub::common {
  class ByteBuffer {
  public:
    ByteBuffer();
    ByteBuffer(size_t initial_capacity);
    ~ByteBuffer();

    ByteBuffer(const ByteBuffer &) = delete;

    ByteBuffer &operator=(const ByteBuffer &) = delete;

    ByteBuffer(ByteBuffer &&other) noexcept;

    ByteBuffer &operator=(ByteBuffer &&other) noexcept;


    Error WriteBytesAt(size_t offset, const uint8_t *data, size_t len);
    Error WriteUInt8At(size_t offset, uint8_t value);
    Error WriteUInt16At(size_t offset, uint16_t value);
    Error WriteUInt32At(size_t offset, uint32_t value);
    Error WriteUInt64At(size_t offset, uint64_t value);

    Error WriteBytes(const uint8_t *data, size_t len);
    Error WriteStr(const char *str, size_t len);
    Error WriteUInt8(uint8_t value);
    Error WriteUInt16(uint16_t value);
    Error WriteUInt32(uint32_t value);
    Error SkipWrite(size_t len);

    Error ReadUInt8(uint8_t &value);
    Error ReadUInt16(uint16_t &value);
    Error ReadUInt32(uint32_t &value);
    Error ReadBytes(uint8_t* dest, size_t len);

    Error SkipBytes(size_t len);
    size_t ReadableBytes() const;
    void ResetReader();

    const uint8_t *Data() const noexcept;
    size_t Size() const noexcept;
    bool Empty() const noexcept;
    void Resize(size_t size);
    void Reserve(size_t capacity);
    void Clear();
    
  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
} // namespace msghub::common
