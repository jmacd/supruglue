// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/pinmap/pinmap.h"
#include "gtest/gtest.h"

TEST(PinmapTest, NamedPins) {
  EXPECT_EQ(21, GPIO_PIN_TO_BIT_NUM(P9_25));
  EXPECT_EQ(3, GPIO_PIN_TO_BANK_NUM(P9_25));

  EXPECT_EQ(21, GPIO_PIN_TO_BIT_NUM(ULED1));
  EXPECT_EQ(1, GPIO_PIN_TO_BANK_NUM(ULED1));
}
