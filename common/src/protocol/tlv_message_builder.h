#pragma once
#include "common/buffer/byte_buffer.h"
#include "common/codec/tlv/tlv_writer.h"
#include "common/protocol/message.h"

namespace msghub::common {

  class TlvMsgBuilder {
   public:
    explicit TlvMsgBuilder(ByteBuffer& buffer);
    ~TlvMsgBuilder();
    TlvMsgBuilder(const TlvMsgBuilder&) = delete;
    TlvMsgBuilder& operator=(const TlvMsgBuilder&) = delete;
    TlvMsgBuilder(TlvMsgBuilder&& other) noexcept;
    TlvMsgBuilder& operator=(TlvMsgBuilder&& other) noexcept;

    TlvMsgBuilder& SetType(MessageType type);
    TlvMsgBuilder& SetSource(ServiceId src);
    TlvMsgBuilder& SetDestination(ServiceId dest);
    TlvMsgBuilder& SetEvent(EventType event);

    TlvMsgBuilder& AddUInt8(TlvType type, uint8_t value);
    TlvMsgBuilder& AddUInt16(TlvType type, uint16_t value);
    TlvMsgBuilder& AddUInt32(TlvType type, uint32_t value);
    TlvMsgBuilder& AddString(TlvType type, const std::string& value);
    TlvMsgBuilder& AddBytes(TlvType type, const uint8_t* data, size_t len);

    Message Build();

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };

}  // namespace msghub::common