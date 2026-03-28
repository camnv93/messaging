#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

#include "buffer/byte_buffer.cpp"
#include "utils/nbo.cpp"

using namespace msg::common;

TEST(ByteBufferTest, ByteBufferTest_WriteU16_TC001) {
  ByteBuffer buff;

  uint16_t write_value = 0x1234;
  buff.WriteUInt16(write_value);
  buff.Dump();
  EXPECT_EQ(buff.Size(), sizeof(write_value));
  auto* data = buff.Data();
  EXPECT_EQ((data == nullptr), false);
  EXPECT_EQ(data[0], 0x12);
  EXPECT_EQ(data[1], 0x34);
}

TEST(ByteBufferTest, ByteBufferTest_WriteU32_TC001) {
  ByteBuffer buff;

  uint32_t write_value = 0x12345678;
  buff.WriteUInt32(write_value);
  buff.Dump();
  EXPECT_EQ(buff.Size(), sizeof(write_value));
  auto* data = buff.Data();
  EXPECT_EQ((data == nullptr), false);
  EXPECT_EQ(data[0], 0x12);
  EXPECT_EQ(data[1], 0x34);
  EXPECT_EQ(data[2], 0x56);
  EXPECT_EQ(data[3], 0x78);
}

TEST(ByteBufferTest, ByteBufferTest_WriteBytes_TC001) {
  ByteBuffer buff;

  uint8_t write_bytes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
  buff.WriteBytes(write_bytes, 10);
  buff.Dump();
  EXPECT_EQ(buff.Size(), 10);
  auto* data = buff.Data();
  EXPECT_EQ((data == nullptr), false);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(data[i], write_bytes[i]);
  }
}

TEST(ByteBufferTest, ByteBufferTest_WriteStr_TC001) {
  ByteBuffer buff;

  std::string write_str = "Hello 1234";
  buff.WriteStr(write_str.data(), write_str.length());
  buff.Dump();
  EXPECT_EQ(buff.Size(), 10);
  auto* data = buff.Data();
  EXPECT_EQ((data == nullptr), false);
  for (int i = 0; i < write_str.length(); i++) {
    EXPECT_EQ(data[i], write_str[i]);
  }
}

TEST(ByteBufferTest, ByteBufferTest_WriteAt_TC001) {
  ByteBuffer buff;
  buff.WriteUInt16(0xFF01);
  buff.Dump();
  uint8_t write_data[] = {0x04,0x05,0x06,0x07};
  buff.WriteAt(6, write_data, 4);
  buff.Dump();
  EXPECT_EQ(buff.Size(), 10);
  auto* data = buff.Data();
  EXPECT_EQ((data == nullptr), false);
  for (int i = 6; i < 10; i++) {
    EXPECT_EQ(data[i], write_data[i-6]);
  }
}