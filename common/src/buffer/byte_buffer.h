//
// Created by devcpp on 3/27/26.
//

#pragma once
#include <cstdint>
#include <memory>


namespace msg::common {
  class ByteBuffer {
  public:
    ByteBuffer();

    ~ByteBuffer();

    ByteBuffer(const ByteBuffer &) = delete;

    ByteBuffer &operator=(const ByteBuffer &) = delete;

    ByteBuffer(ByteBuffer &&other) noexcept;

    ByteBuffer &operator=(ByteBuffer &&other) noexcept;

    void WriteBytes(const uint8_t *data, size_t len);

    void WriteStr(const char *str, size_t len);

    void WriteAt(size_t offset, const uint8_t *data, size_t len);

    void WriteUInt8(uint8_t value);

    void WriteUInt16(uint16_t value);

    void WriteUInt32(uint32_t value);

    void WriteUInt64(uint64_t value);

    const uint8_t *Data() const noexcept;

    size_t Size() const noexcept;

    bool Empty() const noexcept;

    void Reserve(size_t capacity);

    void Clear();

    void Dump() const;
    
  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
} // namespace msg::common
