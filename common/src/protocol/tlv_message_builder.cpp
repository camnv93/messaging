#include "common/protocol/tlv_message_builder.h"
#include "common/buffer/byte_buffer.h"

#include <cassert>

namespace msghub::common {
  struct TlvMsgBuilder::Impl {
   public:
    explicit Impl(ByteBuffer& buffer) : tlv_writer_(buffer), buffer_(buffer) {
      buffer_.Clear();
      buffer_.SkipWrite(sizeof(MessageHeader));
    }
    ~Impl() = default;
    ByteBuffer& buffer_;
    TlvWriter tlv_writer_;
    MessageHeader header_;
    bool is_built_{false};
  };

  TlvMsgBuilder::TlvMsgBuilder(ByteBuffer& buffer)
      : impl_(std::make_unique<Impl>(buffer)) {}
  TlvMsgBuilder::~TlvMsgBuilder() = default;
  TlvMsgBuilder::TlvMsgBuilder(TlvMsgBuilder&& other) noexcept = default;
  TlvMsgBuilder& TlvMsgBuilder::operator=(TlvMsgBuilder&& other) noexcept =
    default;

  TlvMsgBuilder& TlvMsgBuilder::SetType(MessageType type) {
    assert(!impl_->is_built_ && "Cannot set fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->header_.message_type = type;
    return *this;
  }
  TlvMsgBuilder& TlvMsgBuilder::SetSource(ServiceId src) {
    assert(!impl_->is_built_ && "Cannot set fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->header_.source = src;
    return *this;
  }
  TlvMsgBuilder& TlvMsgBuilder::SetDestination(ServiceId dest) {
    assert(!impl_->is_built_ && "Cannot set fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->header_.destination = dest;
    return *this;
  }
  TlvMsgBuilder& TlvMsgBuilder::SetEvent(EventType event) {
    assert(!impl_->is_built_ && "Cannot set fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->header_.event_type = event;
    return *this;
  }

  TlvMsgBuilder& TlvMsgBuilder::AddUInt8(TlvType type, uint8_t value) {
    assert(!impl_->is_built_ && "Cannot add fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->tlv_writer_.WriteUInt8(type, value);
    return *this;
  }
  TlvMsgBuilder& TlvMsgBuilder::AddUInt16(TlvType type, uint16_t value) {
    assert(!impl_->is_built_ && "Cannot add fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->tlv_writer_.WriteUInt16(type, value);
    return *this;
  }
  TlvMsgBuilder& TlvMsgBuilder::AddUInt32(TlvType type, uint32_t value) {
    assert(!impl_->is_built_ && "Cannot add fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->tlv_writer_.WriteUInt32(type, value);
    return *this;
  }
  TlvMsgBuilder& TlvMsgBuilder::AddString(TlvType type,
                                          const std::string& value) {
    assert(!impl_->is_built_ && "Cannot add fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->tlv_writer_.WriteString(type, value);
    return *this;
  }
  TlvMsgBuilder& TlvMsgBuilder::AddBytes(TlvType type, const uint8_t* data,
                                         size_t len) {
    assert(!impl_->is_built_ && "Cannot add fields to a built message");
    if (impl_->is_built_) {
      return *this;
    }
    impl_->tlv_writer_.WriteBytes(type, data, len);
    return *this;
  }
  Message TlvMsgBuilder::Build() {
    assert(!impl_->is_built_ && "Message already built");
    if (impl_->is_built_) {
      return Message(impl_->buffer_.Data(), impl_->buffer_.Size());
    }

    auto& buffer = impl_->buffer_;
    auto& header = impl_->header_;

    uint32_t real_payload_size =
      static_cast<uint32_t>(buffer.Size() - sizeof(MessageHeader));

    size_t offset = 0;
    buffer.WriteUInt16At(offset, header.magic);
    offset += sizeof(header.magic);

    buffer.WriteUInt8At(offset, header.version);
    offset += sizeof(header.version);

    // Header checksum need to be calculated after writing the rest of the header, so we write a placeholder for now
    buffer.WriteUInt8At(offset, 0);
    offset += sizeof(header.header_checksum);

    buffer.WriteUInt8At(offset, static_cast<uint8_t>(header.message_type));
    offset += sizeof(uint8_t);

    buffer.WriteUInt8At(offset, header.ttl);
    offset += sizeof(header.ttl);

    buffer.WriteUInt16At(offset, header.flags);
    offset += sizeof(header.flags);

    buffer.WriteUInt16At(offset, header.event_type);
    offset += sizeof(header.event_type);

    buffer.WriteUInt16At(offset, header.source);
    offset += sizeof(header.source);

    buffer.WriteUInt16At(offset, header.destination);
    offset += sizeof(header.destination);

    // Ghi Payload Size đã tính ở bước 1
    buffer.WriteUInt32At(offset, real_payload_size);
    offset += sizeof(uint32_t);

    PayloadChecksum p_crc = 0;
    // p_crc = Crc32::Calculate(buffer.Data() + sizeof(MessageHeader), real_payload_size);
    buffer.WriteUInt32(p_crc);
    impl_->is_built_ = true;
    return Message(buffer.Data(), buffer.Size());
  }
}  // namespace msghub::common