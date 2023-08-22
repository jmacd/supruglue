#include "include/gpio.h"
#include "gtest/gtest.h"

TEST(SocTest, WordSize) {
  // This is a 32-bit platform
  EXPECT_EQ(sizeof(uint32_t), WORDSZ);
}
