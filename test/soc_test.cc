#include "include/soc.h"
#include "gtest/gtest.h"

TEST(SocTest, WordSize) {
  int sz = sizeof(uint32_t);
  EXPECT_EQ(WORDSZ, sz);
}

TEST(SocTest, NumWords) {
  uint32_t words[4];

  EXPECT_EQ(SIZEOF_WORDS(words), 4);
}
