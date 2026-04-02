#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

#include "buffer/byte_buffer.cpp"
#include "codec/tlv/tlv_writer.cpp"
#include "codec/tlv/tlv_reader.cpp"
#include "codec/tlv/tlv_viewer.cpp"
#include "utils/hex2bin.cpp"
#include "utils/nbo.cpp"
#include "utils/bit_convert.cpp"
#include "protocol/tlv_message_builder.cpp"
#include "protocol/message.cpp"
#include "protocol/message_header.h"

using namespace msghub::common;

TEST(TlvMsgBuilder, TlvMsgBuilder_Basic_TC001) {
  ByteBuffer buffer;
  TlvMsgBuilder builder(buffer);
  auto msg = builder.SetType(MessageType::SUBSCRIBE)
      .SetSource(0x01020304)
      .SetDestination(0x04030201)
      .SetEvent(0x10)
      .AddUInt8(0xA1, 0xFF)
      .AddUInt16(0xA2, 0x1234)
      .AddUInt32(0xA3, 0x12345678)
      .AddString(0xA4, "Hello")
      .Build();

  EXPECT_EQ(msg.GetHeader().message_type, MessageType::SUBSCRIBE);
  EXPECT_EQ(msg.GetHeader().source, 0x01020304);
  EXPECT_EQ(msg.GetHeader().destination, 0x04030201);
  EXPECT_EQ(msg.GetHeader().event_type, 0x10);
}