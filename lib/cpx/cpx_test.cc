// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/cpx.h"
#include "gtest/gtest.h"
#include <setjmp.h>

using std::vector;

ThreadID t2id(Thread *t) {
  return reinterpret_cast<ThreadID>(t);
}

TEST(CpxTest, TwoThreads) {
  System sys;
  Thread threads[2];
  uint32_t regs[2] = {0, 0};
  uint8_t stack0[1024];
  uint8_t stack1[1024];

  EXPECT_EQ(0, Init(&sys, DefaultSystemConfig()));

  EXPECT_EQ(0, Create(
                   &sys, &threads[0], stack0, [](System *sys, void *arg) { *(uint32_t *)arg = 1; }, (void *)&regs[0],
                   DefaultThreadConfig()));
  EXPECT_EQ(0, Create(
                   &sys, &threads[1], stack1, [](System *sys, void *arg) { *(uint32_t *)arg = 2; }, (void *)&regs[1],
                   DefaultThreadConfig()));

  EXPECT_EQ(0, ::Run(&sys));
  EXPECT_EQ(1, regs[0]);
  EXPECT_EQ(2, regs[1]);
}

extern "C" {
void run_func(System *sys, void *arg) {
  for (int i = 0; i < 2; i++) {
    fprintf(stderr, "loop body %d\n", i);
    vector<ThreadID> *which = reinterpret_cast<vector<ThreadID> *>(arg);
    which->push_back(PID(sys));
    Yield(sys);
  }
  fprintf(stderr, "body return\n");
}
}

TEST(CpxTest, TwiceAlternating) {
  System sys;
  Thread threads[2];
  vector<ThreadID> which;
  uint8_t stack0[1024];
  uint8_t stack1[1024];

  EXPECT_EQ(0, Init(&sys, DefaultSystemConfig()));

  EXPECT_EQ(0, Create(&sys, &threads[0], stack0, run_func, &which, DefaultThreadConfig()));
  EXPECT_EQ(0, Create(&sys, &threads[1], stack1, run_func, &which, DefaultThreadConfig()));

  EXPECT_EQ(0, ::Run(&sys));

  auto id0 = t2id(&threads[0]);
  auto id1 = t2id(&threads[1]);

  vector<ThreadID> expect = {
      id0,
      id1,
      id0,
      id1,
  };

  EXPECT_EQ(expect, which);
}
