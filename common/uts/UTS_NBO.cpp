#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "utils/nbo.cpp"

using namespace msghub::common;

TEST(nbotest, to_network_u16_tc001) {
  uint16_t host = 0x1234;
  uint16_t expect = 0x3412;
  auto net = nbo::to_network_u16(host);
  EXPECT_EQ(net, expect);
}
TEST(nbotest, to_network_u16_tc002) {
  uint16_t host = 0x00FF;
  uint16_t expect = 0xFF00;
  auto net = nbo::to_network_u16(host);
  EXPECT_EQ(net, expect);
}

TEST(nbotest, to_network_u16_tc003) {
  uint16_t host = 0xFF00;
  uint16_t expect = 0x00FF;
  auto net = nbo::to_network_u16(host);
  EXPECT_EQ(net, expect);
}