#include "lib/gpio/gpio.h"
#include "lib/pinmap/pinmap-defs.h"
#include "lib/pinmap/pinmap.h"
#include "gtest/gtest.h"

const uint32_t allbits = 0xffffffff;
const uint32_t halfbits = 0x0ff00ff0;

TEST(GpioTest, Register) {
  test_system *soc = new test_system;

  for (int i = 0; i < GPIO_NUM_REGISTERS; i++) {
    gpio_bank *gpio = reinterpret_cast<gpio_bank *>(&soc->gpio_banks[i]);

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
  test_system *soc = new test_system;

  gpio_pin pin = GPIO_PIN(soc, P9_25);
  EXPECT_EQ(GPIO_BANK3(soc), pin.bank);
  EXPECT_EQ(GPIO_PIN_TO_BIT_NUM(P9_25), pin.bit);

  pin = GPIO_PIN(soc, P8_3);
  EXPECT_EQ(GPIO_BANK1(soc), pin.bank);
  EXPECT_EQ(GPIO_PIN_TO_BIT_NUM(P8_3), pin.bit);

  pin = GPIO_PIN(soc, ULED2);
  EXPECT_EQ(GPIO_BANK1(soc), pin.bank);
  EXPECT_EQ(GPIO_PIN_TO_BIT_NUM(ULED2), pin.bit);
}

TEST(GpioTest, PinmapMacros) {
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
