#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>


#include "utils/bit_convert.cpp"
#include "buffer/byte_buffer.cpp"
#include "codec/tlv/tlv_writer.cpp"
#include "utils/hex2bin.cpp"
#include "utils/nbo.cpp"


using namespace msghub::common;
TEST(TlvWriter, TlvWriter_UINT8_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteUInt8(0x0102, 0xFF);
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}
TEST(TlvWriter, TlvWriter_UINT16_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteUInt16(0x0102, 0x1122);
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}

TEST(TlvWriter, TlvWriter_UINT32_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteUInt32(0x0102, 0x11223344);
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}

TEST(TlvWriter, TlvWriter_NESTED_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.Begin(0xAABB);
  writer.WriteUInt32(0xCCDD, 0x11223344);
  writer.End();
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}
TEST(TlvWriter, TlvWriter_NESTED_TC002) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.Begin(0xAFBF);
  uint8_t bytes[15] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0C, 0x0D, 0x0E, 0x0F};
  writer.WriteBytes(0xFAFA, bytes, sizeof(bytes));
  writer.End();
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}
TEST(TlvWriter, TlvWriter_NESTED_TC003) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.Begin(0xA1A1);
  writer.WriteUInt8(0xC1C1, 2);
  writer.Begin(0xA2A2);
  writer.WriteUInt8(0xC2C2, 2);
  writer.Begin(0xA3A3);
  writer.WriteUInt8(0xC3C3, 3);
  writer.Begin(0xA4A4);
  writer.WriteUInt8(0xC4C4, 4);
  writer.End();
  writer.End();
  writer.End();

  writer.End();
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}

TEST(TlvWriter, TlvWriter_MUTIL_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteUInt8(0xC1C1, 1);
  writer.WriteUInt16(0xC2C2, 2);
  writer.WriteUInt32(0xC3C3, 3);
  writer.WriteUInt8(0xC4C4, 1);
  writer.WriteUInt16(0xC5C5, 2);
  writer.WriteUInt32(0xC6C6, 3);
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}

TEST(TlvWriter, TlvWriter_EMPTY_TC001) {
  std::vector<uint8_t> data;
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteBytes(0x01, data.data(), data.size());
  std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;
}