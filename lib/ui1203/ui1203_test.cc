// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "ui1203.h"
#include "gtest/gtest.h"

TEST(Ui1203Test, Bits) {
  for (unsigned char c = 0; c < 128; c++) {
    int32_t bits = asciiToBits(c);
    uint8_t ascii = bitsToAscii(bits);

    EXPECT_EQ(c, ascii);
  }
}
