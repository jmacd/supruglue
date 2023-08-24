#include "include/gpio.h"
#include "gtest/gtest.h"

const uint32_t allbits = 0xffffffff;

TEST(GpioTest, Output) {
  GPIO_BANK1[GPIOREG_CLEARDATAOUT] = allbits;

  EXPECT_EQ(allbits, GPIO_BANK1[GPIOREG_DATAOUT]);
}
