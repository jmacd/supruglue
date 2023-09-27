// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/cpx.h"
#include "gtest/gtest.h"

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

TEST(CpxTest, FiveTimes) {
  System sys;
  Thread threads[2];
  uint32_t regs[2] = {0, 0};

  auto five =
      [](void *arg) {
        for (int i = 0; i < 5; i++) {
          (*(uint32_t *)arg) += 1;
          Yield();
        }
        return;
      }

  EXPECT_EQ(0, Init(&sys, DefaultSystemConfig()));

  EXPECT_EQ(0, Create(&sys, &threads[0], five, (void *)&regs[0], DefaultThreadConfig()));
  EXPECT_EQ(0, Create(&sys, &threads[1], five, (void *)&regs[1], DefaultThreadConfig()));

  EXPECT_EQ(0, ::Run(&sys));
  EXPECT_EQ(5, regs[0]);
  EXPECT_EQ(5, regs[1]);
}
