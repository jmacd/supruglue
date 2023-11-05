// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "supruglue/test32/gpio.h"
#include "supruglue/test32/pinmap.h"
#include "gtest/gtest.h"

TEST(PinMapTest, GpioMacros) {
  test_system *soc = new test_system;

  EXPECT_EQ(3, GPIO_PIN_TO_BANK_NUM(P9_25));
  EXPECT_EQ(21, GPIO_PIN_TO_BIT_NUM(P9_25));
  EXPECT_EQ(GPIO_BANK3(soc), GPIO_PIN_TO_REGISTER(soc, P9_25));

  EXPECT_EQ(1, GPIO_PIN_TO_BANK_NUM(P8_3));
  EXPECT_EQ(6, GPIO_PIN_TO_BIT_NUM(P8_3));
  EXPECT_EQ(GPIO_BANK1(soc), GPIO_PIN_TO_REGISTER(soc, P8_3));

  EXPECT_EQ(1, GPIO_PIN_TO_BANK_NUM(ULED1));
  EXPECT_EQ(21, GPIO_PIN_TO_BIT_NUM(ULED1));
  EXPECT_EQ(GPIO_BANK1(soc), GPIO_PIN_TO_REGISTER(soc, ULED1));
}
