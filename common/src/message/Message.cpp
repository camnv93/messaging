//
// Created by devcpp on 3/26/26.
//

#include "Message.h"

#include <utility>
#include <vector>
#include <stdexcept>

#define CHECK_NULL(object) \
  do { \
    if ((object) == nullptr) { \
      throw std::runtime_error( \
        std::string("Null pointer detected: ") + #object + \
        " at " + __FILE__ + ":" + std::to_string(__LINE__) \
      ); \
    } \
  } while (0)

namespace msg::common {
  struct Message::Impl {
  public:
    Impl() = default;

    ~Impl() = default;

    MessageHeader header_;
    std::vector<uint8_t> payload_;

  private:
  };

  Message::Message() : impl_(std::make_shared<Impl>()) {
  }

  Message::~Message() = default;

  Message::Message(const Message &other) = default;

  Message &Message::operator=(const Message &other) = default;

  Message &Message::operator=(Message &&other) noexcept = default;

  Message::Message(Message &&other) noexcept = default;


  void Message::SetMsgType(MessageType message_type) {
    CHECK_NULL(impl_);
    impl_->header_.message_type = message_type;
  }

  MessageType Message::GetMsgType() const {
    CHECK_NULL(impl_);
    return impl_->header_.message_type;
  }


  void Message::SetEvent(EventType event) {
    CHECK_NULL(impl_);
    impl_->header_.event_type = event;
  }

  EventType Message::GetEvent() const {
    CHECK_NULL(impl_);
    return impl_->header_.event_type;
  }

  void Message::SetSource(ServiceId source) {
    CHECK_NULL(impl_);
    impl_->header_.source = source;
  }

  ServiceId Message::GetSource() const {
    CHECK_NULL(impl_);
    return impl_->header_.source;
  }

  void Message::SetPayload(const std::vector<uint8_t> &payload) {
    CHECK_NULL(impl_);
    impl_->payload_ = payload;
  }

  void Message::SetPayload(const uint8_t *payload, size_t size) {
    CHECK_NULL(impl_);
    CHECK_NULL(payload);
    impl_->payload_.assign(payload, payload + size);;
  }

  std::vector<uint8_t> Message::GetPayload() const {
    CHECK_NULL(impl_);
    return impl_->payload_;
  }

  size_t Message::PayloadSize() const {
    return impl_->payload_.size();
  }
}
