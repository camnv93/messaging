#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

#include "buffer/byte_buffer.cpp"
#include "codec/tlv/tlv_reader.cpp"
#include "codec/tlv/tlv_writer.cpp"
#include "codec/tlv/tlv_viewer.cpp"
#include "utils/hex2bin.cpp"
#include "utils/nbo.cpp"
#include "utils/bit_convert.cpp"
using namespace msghub::common;


TEST(TlvReader, TlvWriter_ReadUint16_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteUInt8(0xC0C1, 0x1F);
  TlvReader reader(buffer.Data(), buffer.Size());
  auto tlv_viewr = reader.Next();
  EXPECT_TRUE(tlv_viewr.has_value());
  EXPECT_EQ(tlv_viewr->header.type, 0xC0C1);
  EXPECT_EQ(tlv_viewr->header.length, 1);
  EXPECT_EQ(tlv_viewr->GetUInt8(), 0x1F);
}

TEST(TlvReader, TlvWriter_ReadUInt16_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteUInt16(0xC0C1, 0x1A1F);
  TlvReader reader(buffer.Data(), buffer.Size());
  auto tlv_viewr = reader.Next();
  EXPECT_TRUE(tlv_viewr.has_value());
  EXPECT_EQ(tlv_viewr->header.type, 0xC0C1);
  EXPECT_EQ(tlv_viewr->header.length, 2);
  EXPECT_EQ(tlv_viewr->GetUInt16(), 0x1A1F);
}

TEST(TlvReader, TlvWriter_ReadUInt32_TC001) {
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteUInt32(0xC0C1, 0x1A1FA1F1);
  TlvReader reader(buffer.Data(), buffer.Size());
  auto tlv_viewr = reader.Next();
  EXPECT_TRUE(tlv_viewr.has_value());
  EXPECT_EQ(tlv_viewr->header.type, 0xC0C1);
  EXPECT_EQ(tlv_viewr->header.length, 4);
  EXPECT_EQ(tlv_viewr->GetUInt32(), 0x1A1FA1F1);
}

TEST(TlvReader, TlvWriter_ReadStr_TC001) {
  const std::string str = "Hello, World!";
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteString(0xC0C1, str);
  TlvReader reader(buffer.Data(), buffer.Size());
  auto tlv_viewr = reader.Next();
  EXPECT_TRUE(tlv_viewr.has_value());
  EXPECT_EQ(tlv_viewr->header.type, 0xC0C1);
  EXPECT_EQ(tlv_viewr->header.length, str.length());
  EXPECT_EQ(tlv_viewr->GetString(), str);
}

TEST(TlvReader, TlvWriter_ReadStr_TC002) {
  const std::string str = "Hello, World!";
  const std::string expect_str = "Hello";
  ByteBuffer buffer;
  TlvWriter writer(buffer);
  writer.WriteString(0xC0C1, str);
  TlvReader reader(buffer.Data(), buffer.Size());
  auto tlv_viewr = reader.Next();
  EXPECT_TRUE(tlv_viewr.has_value());
  EXPECT_EQ(tlv_viewr->header.type, 0xC0C1);
  EXPECT_EQ(tlv_viewr->header.length, str.length());
  EXPECT_EQ(tlv_viewr->GetString(expect_str.length()), expect_str);
}

// TEST(TlvReader, TlvWriter_MUTIL_TC001) {

//   std::vector<TlvHeader> list_header;
//   ByteBuffer buffer;
//   TlvWriter writer(buffer);
//   writer.WriteUInt8(0xC0C1, 1);
//   list_header.push_back({0xC0C1, 1});
//   writer.WriteUInt16(0xC0C2, 2);
//   list_header.push_back({0xC0C2, 2});
//   writer.WriteUInt32(0xC0C3, 3);
//   list_header.push_back({0xC0C3, 4});
//   writer.WriteUInt8(0xC0C4, 1);
//   list_header.push_back({0xC0C4, 1});
//   writer.WriteUInt16(0xC0C5, 2);
//   list_header.push_back({0xC0C5, 2});
//   writer.WriteUInt32(0xC0C6, 3);
//   list_header.push_back({0xC0C6, 4});
//   writer.Begin(0xB0B1);
//   list_header.push_back({0xB0B1, 20});
//   writer.WriteUInt32(0xC0C7, 3);
//   writer.WriteUInt32(0xC0C8, 4);
//   writer.End();
//   //std::cout << hex2bin::Bin2Hex(buffer.Data(), buffer.Size()) << std::endl;

//   TlvReader reader(buffer.Data(), buffer.Size());
  
//   int toltal = 0;
//   while (reader.HasNext()) {
//     auto tlv_viewr = reader.Next();
//     EXPECT_EQ(list_header.at(toltal).type, tlv_viewr->header.type);
//     EXPECT_EQ(list_header.at(toltal).length, tlv_viewr->header.length);
//     std::cout << hex2bin::Bin2Hex(tlv_viewr->data, tlv_viewr->header.length) << std::endl;
//     toltal ++;
//   }
//   EXPECT_EQ(toltal, 7);
// }