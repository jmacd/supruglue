#include "gtest/gtest.h"

extern "C" {
#include "soc.h"
}

TEST(SocTest, WordSize) {
  int sz = sizeof(uint32_t);
  EXPECT_EQ(WORDSZ, sz);
}

TEST(SocTest, NumWords) {
  uint32_t words[4];

  EXPECT_EQ(SIZEOF_WORDS(words), 4);
}

TEST(SocTest, TestSOC) {
  supruglue_test_system *ts = Test_New_SOC();
  EXPECT_NE(ts, nullptr);
}
