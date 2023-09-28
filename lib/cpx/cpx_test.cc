// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/cpx.h"
#include "gtest/gtest.h"
#include <setjmp.h>

using std::vector;

TEST(CpxTest, TwoThreads) {
  System sys;
  Thread threads[2];
  uint32_t regs[2] = {0, 0};

  EXPECT_EQ(0, Init(&sys, DefaultSystemConfig()));

  EXPECT_EQ(0,
            Create(
                &sys, &threads[0], [](void *arg) { *(uint32_t *)arg = 1; }, (void *)&regs[0], DefaultThreadConfig()));
  EXPECT_EQ(0,
            Create(
                &sys, &threads[1], [](void *arg) { *(uint32_t *)arg = 2; }, (void *)&regs[1], DefaultThreadConfig()));

  EXPECT_EQ(0, ::Run(&sys));
  EXPECT_EQ(1, regs[0]);
  EXPECT_EQ(2, regs[1]);
}

TEST(CpxTest, TwiceAlternating) {
  System sys;
  Thread threads[2];
  vector<ProcessID> which;

  auto run = [](void *arg) {
    for (int i = 0; i < 2; i++) {
      reinterpret_cast<vector<ProcessID> *>(arg)->push_back(Pid());
      Yield();
    }
  };

  EXPECT_EQ(0, Init(&sys, DefaultSystemConfig()));

  EXPECT_EQ(0, Create(&sys, &threads[0], run, &which, DefaultThreadConfig()));
  EXPECT_EQ(0, Create(&sys, &threads[1], run, &which, DefaultThreadConfig()));

  EXPECT_NE(threads[0].id, threads[1].id);

  EXPECT_EQ(0, ::Run(&sys));

  vector<ProcessID> expect = {
      threads[0].id,
      threads[1].id,
      threads[0].id,
      threads[1].id,
  };

  EXPECT_EQ(expect, which);
}
