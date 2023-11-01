// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/thread/thread.h"
#include "gtest/gtest.h"

TEST(Thread, NameAndID) {
  Thread  th;
  uint8_t stack[100];

  th.cfg = NewThreadConfig("hello", stack, sizeof(stack));

  EXPECT_STREQ("hello", th.cfg.name);
  EXPECT_NE(0, TID(&th));
}
