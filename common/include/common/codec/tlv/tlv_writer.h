// Created by devcpp on 3/27/26.
// tlv_writer.h

#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include "common/buffer/byte_buffer.h"
#include "common/utils/error.h"
#include "common/codec/tlv/tlv.h"

namespace msghub::common {
  class TlvWriter {
   public:
    explicit TlvWriter(ByteBuffer& buffer);
    ~TlvWriter();

    TlvWriter(const TlvWriter &) = delete;
    TlvWriter &operator=(const TlvWriter &) = delete;

    TlvWriter(TlvWriter &&other) noexcept;
    TlvWriter &operator=(TlvWriter &&other) noexcept;

    Error WriteUInt8(TlvType type, uint8_t value);
    Error WriteUInt16(TlvType type, uint16_t value);
    Error WriteUInt32(TlvType type, uint32_t value);
    Error WriteString(TlvType type, const std::string& value);
    Error WriteBytes(TlvType type, const uint8_t* data, size_t len);

    Error Begin(TlvType type);
    Error End();

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::common