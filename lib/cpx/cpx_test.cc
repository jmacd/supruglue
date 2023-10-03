// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/cpx.h"
#include "lib/cpx/cpx_test_c.h"
#include "gtest/gtest.h"
#include <setjmp.h>

using std::vector;

vector<ThreadID> test_run_get(void) {
  vector<ThreadID> res;
  for (int i = 0; i < test_run_vector_size; i++) {
    res.push_back(test_run_vector[i]);
  }
  return res;
}

TEST(CpxTest, TwiceAlternating) {
  System sys;
  Thread threads[2];
  uint8_t stack0[1024];
  uint8_t stack1[1024];

  EXPECT_EQ(0, Init(&sys, DefaultSystemConfig()));

  EXPECT_EQ(0, Create(&sys, &threads[0], &test_run_func, "2", 2, DefaultThreadConfig(stack0, 1024)));
  EXPECT_EQ(0, Create(&sys, &threads[1], &test_run_func, "22", 3, DefaultThreadConfig(stack1, 1024)));

  EXPECT_EQ(0, ::Run(&sys));

  auto id0 = TID(&threads[0]);
  auto id1 = TID(&threads[1]);

  vector<ThreadID> expect = {
      id0,
      id1,
      id0,
      id1,
  };

  EXPECT_EQ(expect, test_run_get());
}
