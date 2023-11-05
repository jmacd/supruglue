// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "supruglue/test32/gpio.h"
#include "supruglue/test32/pinmap.h"
#include "supruglue/test32/soc.h"
#include "ui1203.h"
#include "gtest/gtest.h"

TEST(Ui1203Test, Output) {
  test_system *soc = new test_system;

  UI1203_Reader rdr;
  UI1203_Init_Reader(&rdr, GPIO_PIN(soc, P8_3), GPIO_PIN(soc, P9_25));
}
