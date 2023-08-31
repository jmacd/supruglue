// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

extern "C" {
#include "supruglue/test32/gpio.h"
#include "supruglue/test32/pinmap.h"
#include "ui1203.h"
}

TEST(Ui1203Test, Output) {
  test_system *soc = Test_New_SOC();

  // @@@
  NewReader(GPIO_PIN(soc, P8_3), GPIO_PIN(soc, P9_25));
}
