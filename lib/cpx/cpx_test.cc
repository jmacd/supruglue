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

typedef struct {
  Thread thread;
  uint32_t index;
  uint32_t *regs;
} TestThread;

TEST(CpxTest, TwoThreads) {
  TestThread threads[2];
  uint32_t regs[2] = {0, 0};

  threads[0].index = 0;
  threads[1].index = 1;
  threads[0].regs = regs;
  threads[1].regs = regs;

  uint8_t stack0[1024];
  uint8_t stack1[1024];

  auto test_func = [](ThreadID tid, Args args) {
    TestThread *tt = (TestThread *)tid;
    tt->regs[tt->index] = tt->index + 1;
  };

  EXPECT_EQ(0, Init(DefaultSystemConfig()));

  EXPECT_EQ(0, Create(&threads[0].thread, test_func, Args{.ptr = "1"}, DefaultThreadConfig(stack0, 1024)));
  EXPECT_EQ(0, Create(&threads[1].thread, test_func, Args{.ptr = "2"}, DefaultThreadConfig(stack1, 1024)));

  EXPECT_EQ(0, ::Run());
  EXPECT_EQ(1, regs[0]);
  EXPECT_EQ(2, regs[1]);
}

TEST(CpxTest, TwiceAlternating) {
  Thread threads[2];
  uint8_t stack0[1024];
  uint8_t stack1[1024];

  EXPECT_EQ(0, Init(DefaultSystemConfig()));

  EXPECT_EQ(0, Create(&threads[0], test_run_func, Args{.ptr = "2"}, DefaultThreadConfig(stack0, 1024)));
  EXPECT_EQ(0, Create(&threads[1], test_run_func, Args{.ptr = "22"}, DefaultThreadConfig(stack1, 1024)));

  EXPECT_EQ(0, ::Run());

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

TEST(CpxTest, TestOverflow) {
  Thread thread;
  uint8_t stack0[500];

  EXPECT_EQ(0, Init(DefaultSystemConfig()));

  EXPECT_EQ(0, Create(&thread, test_recursive_func, Args{.ptr = "9"}, DefaultThreadConfig(stack0, sizeof(stack0))));

  EXPECT_EQ(0, ::Run());

  // @@@
}
