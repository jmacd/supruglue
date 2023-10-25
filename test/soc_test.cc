#include "supruglue/test32/soc.h"
#include "gtest/gtest.h"

TEST(SocTest, WordSize) {
  int sz = sizeof(uint32_t);
  EXPECT_EQ(WORDSZ, sz);
}

TEST(SocTest, NumWords) {
  uint32_t words[4];

  EXPECT_EQ(SIZEOF_WORDS(words), 4);
}

TEST(SocTest, TestSOC) {
  test_system *ts = new test_system(0);
  EXPECT_NE(ts, nullptr);
  EXPECT_EQ(0, PRU_CORE_NUMBER(ts));
  delete ts;

  ts = new test_system(1);
  EXPECT_NE(ts, nullptr);
  EXPECT_EQ(1, PRU_CORE_NUMBER(ts));
}
