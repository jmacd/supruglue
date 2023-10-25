#include "supruglue/test32/gpio.h"
#include "supruglue/test32/pinmap.h"
#include "gtest/gtest.h"

const uint32_t allbits = 0xffffffff;
const uint32_t halfbits = 0x0ff00ff0;

TEST(GpioTest, Register) {
  test_system *soc = new test_system(0);

  for (int i = 0; i < GPIO_NUM_REGISTERS; i++) {
    gpio_register *gpio = &soc->gpio_banks[i];

    // Clear al bits
    GPIO_SetRegister(gpio, GPIOREG_CLEARDATAOUT, allbits);
    EXPECT_EQ(0, GPIO_GetRegister(gpio, GPIOREG_DATAOUT));

    // Set all bits
    GPIO_SetRegister(gpio, GPIOREG_SETDATAOUT, allbits);
    EXPECT_EQ(allbits, GPIO_GetRegister(gpio, GPIOREG_DATAOUT));

    // Clear half bits; expect other half set
    GPIO_SetRegister(gpio, GPIOREG_CLEARDATAOUT, ~halfbits);
    EXPECT_EQ(halfbits, GPIO_GetRegister(gpio, GPIOREG_DATAOUT));

    // Set the half that was already set
    GPIO_SetRegister(gpio, GPIOREG_SETDATAOUT, halfbits);
    EXPECT_EQ(halfbits, GPIO_GetRegister(gpio, GPIOREG_DATAOUT));

    // Clear the half that are set; expect zero
    GPIO_SetRegister(gpio, GPIOREG_CLEARDATAOUT, halfbits);
    EXPECT_EQ(0, GPIO_GetRegister(gpio, GPIOREG_DATAOUT));

    // Set half the bits
    GPIO_SetRegister(gpio, GPIOREG_DATAIN, halfbits);
    EXPECT_EQ(halfbits, GPIO_GetRegister(gpio, GPIOREG_DATAOUT));

    // Set the other half of the bits
    GPIO_SetRegister(gpio, GPIOREG_DATAIN, ~halfbits);
    EXPECT_EQ(~halfbits, GPIO_GetRegister(gpio, GPIOREG_DATAOUT));
  }
}

TEST(GpioTest, Ctor) {
  test_system *soc = new test_system(0);

  gpio_pin pin = GPIO_PIN(soc, P9_25);

  EXPECT_EQ(GPIO_REGISTER3(soc), pin.reg);
  EXPECT_EQ(GPIO_PIN_TO_BIT(P9_25), pin.bit);

  pin = GPIO_PIN(soc, P8_3);

  EXPECT_EQ(GPIO_REGISTER1(soc), pin.reg);
  EXPECT_EQ(GPIO_PIN_TO_BIT(P8_3), pin.bit);
}
