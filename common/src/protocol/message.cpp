// message.cpp
//

#include "message.h"

#include <cassert>
#include <cstring>

#include "common/protocol/payload.h"
#include "common/utils/bit_convert.h"

namespace msghub::common {
  struct Message::Impl {
   public:
    Impl(const uint8_t* raw_data, size_t len)
        : raw_ptr_(raw_data), total_len_(len), is_parsed_(false) {}
    ~Impl() = default;

    Error Parse() {
      if (is_parsed_) {
        return Error::Ok;
      }
      if (raw_ptr_ == nullptr || total_len_ == 0) {
        return Error::Buffer_Empty;
      }
      this->parse_error_ = this->ParseHeader();
      if (this->parse_error_ != Error::Ok) {
        return this->parse_error_;
      }
      this->parse_error_ = this->ParsePayload();
      if (this->parse_error_ != Error::Ok) {
        return this->parse_error_;
      }
      this->parse_error_ = this->ParseChecksum();
      is_parsed_ = true;
      return this->parse_error_;
    }

    Error ParseHeader() {
      if (total_len_ < sizeof(MessageHeader)) {
        return Error::Message_InvalidHeader;
      }
      size_t header_offset = 0;
      header_.magic = bit::GetUInt16(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.magic);

      header_.version = bit::GetUInt8(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.version);

      header_.header_checksum = bit::GetUInt8(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.header_checksum);

      header_.message_type =
        static_cast<MessageType>(bit::GetUInt8(raw_ptr_ + header_offset));
      header_offset += sizeof(header_.message_type);

      header_.ttl = bit::GetUInt8(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.ttl);

      header_.flags = bit::GetUInt16(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.flags);

      header_.event_type = bit::GetUInt16(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.event_type);

      header_.source = bit::GetUInt32(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.source);

      header_.destination = bit::GetUInt32(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.destination);

      header_.payload_size = bit::GetUInt32(raw_ptr_ + header_offset);
      header_offset += sizeof(header_.payload_size);
      return Error::Ok;
    }

    Error ParsePayload() {
      size_t payload_offset = sizeof(MessageHeader);
      if (total_len_ < (payload_offset + sizeof(PayloadChecksum))) {
        return Error::Message_InvalidPayload;
      }
      size_t payload_size =
        total_len_ - payload_offset - sizeof(PayloadChecksum);
      payload_ = Payload(raw_ptr_ + payload_offset, payload_size);
      return Error::Ok;
    }

    Error ParseChecksum() {
      size_t checksum_offset = sizeof(MessageHeader) + payload_.Size();
      if (total_len_ < (checksum_offset + sizeof(PayloadChecksum))) {
        return Error::Message_InvalidChecksum;
      }
      payload_checksum_ = bit::GetUInt32(raw_ptr_ + checksum_offset);
      return Error::Ok;
    }

   public:
    const uint8_t* raw_ptr_;
    size_t total_len_;
    bool is_parsed_{false};
    Error parse_error_{Error::Unknown};

   public:
    MessageHeader header_;
    Payload payload_;
    PayloadChecksum payload_checksum_{0};
  };

  Message::Message(const uint8_t* raw_data, size_t len)
      : impl_(std::make_unique<Impl>(raw_data, len)) {}

  Message::Message(Message&&) noexcept = default;
  Message& Message::operator=(Message&&) noexcept = default;
  Message::~Message() = default;

  Error Message::Parse() {
    return impl_->Parse();
  }
  
  Error Message::GetError() const {
    return impl_->parse_error_;
  }
  MessageHeader Message::GetHeader() const {
    if (!impl_->is_parsed_ && impl_->parse_error_ != Error::Ok) {
      return MessageHeader();
    }
    return impl_->header_;
  }

  Payload Message::GetPayload() const {
    if (!impl_->is_parsed_ && impl_->parse_error_ != Error::Ok) {
      return Payload();
    }
    return impl_->payload_;
  }

  PayloadChecksum Message::GetPayloadChecksum() const {
    if (!impl_->is_parsed_ && impl_->parse_error_ != Error::Ok) {
      return 0;
    }
    return impl_->payload_checksum_;
  }

  const uint8_t* Message::RawData() const {
    return impl_->raw_ptr_;
  }

  size_t Message::TotalSize() const {
    return impl_->total_len_;
  }
}  // namespace msghub::common