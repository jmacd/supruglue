// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

extern "C" {
#include "supruglue/test32/gpio.h"
#include "supruglue/test32/pinmap.h"
}

TEST(PinMapTest, GpioMacros) {
  test_system *soc = Test_New_SOC();

  EXPECT_EQ(3, GPIO_PIN_TO_BANK(P9_25));
  EXPECT_EQ(21, GPIO_PIN_TO_BIT(P9_25));
  EXPECT_EQ(GPIO_REGISTER3(soc), GPIO_PIN_TO_REGISTER(soc, P9_25));

  EXPECT_EQ(1, GPIO_PIN_TO_BANK(P8_3));
  EXPECT_EQ(6, GPIO_PIN_TO_BIT(P8_3));
  EXPECT_EQ(GPIO_REGISTER1(soc), GPIO_PIN_TO_REGISTER(soc, P8_3));
}
