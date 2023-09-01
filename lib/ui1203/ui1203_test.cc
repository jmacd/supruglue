// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#include "supruglue/test32/gpio.h"
#include "supruglue/test32/pinmap.h"
#include "supruglue/test32/soc.h"

extern "C" {
#include "ui1203.h"
}

TEST(Ui1203Test, Output) {
  test_system *soc = new test_system;

  UI1203_Reader rdr;
  UI1203_Init_Reader(&rdr, GPIO_PIN(soc, P8_3), GPIO_PIN(soc, P9_25));

  soc->Start([&rdr]() -> void { UI1203_Run_Reader(&rdr); });
}
