#include "lib/gpio/gpio.h"
#include "lib/pinmap/pinmap.h"
#include "lib/soc/soc.h"
#include "gtest/gtest.h"

const uint32_t allbits = 0xffffffff;
const uint32_t halfbits = 0x0ff00ff0;

class GpioTest : public testing::Test {
protected:
  void SetUp() override {
    SystemOnChipSetup();
  }
};

TEST_F(GpioTest, Register) {
  gpio_bank *banks[] = {
      GPIO_BANK0,
      GPIO_BANK1,
      GPIO_BANK2,
      GPIO_BANK3,
  };

  for (int i = 0; i < GPIO_NUM_REGISTERS; i++) {
    gpio_bank *gpio = banks[i];

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

TEST_F(GpioTest, Ctor) {
  gpio_pin pin = GPIO_PIN(P9_25);
  EXPECT_EQ(GPIO_BANK3, pin.bank);
  EXPECT_EQ(GPIO_PIN_TO_BIT_NUM(P9_25), pin.bit);

  pin = GPIO_PIN(P8_3);
  EXPECT_EQ(GPIO_BANK1, pin.bank);
  EXPECT_EQ(GPIO_PIN_TO_BIT_NUM(P8_3), pin.bit);

  pin = GPIO_PIN(ULED2);
  EXPECT_EQ(GPIO_BANK1, pin.bank);
  EXPECT_EQ(GPIO_PIN_TO_BIT_NUM(ULED2), pin.bit);
}

TEST_F(GpioTest, PinmapMacros) {
  EXPECT_EQ(3, GPIO_PIN_TO_BANK_NUM(P9_25));
  EXPECT_EQ(21, GPIO_PIN_TO_BIT_NUM(P9_25));
  EXPECT_EQ(GPIO_BANK3, GPIO_PIN_TO_REGISTER(P9_25));

  EXPECT_EQ(1, GPIO_PIN_TO_BANK_NUM(P8_3));
  EXPECT_EQ(6, GPIO_PIN_TO_BIT_NUM(P8_3));
  EXPECT_EQ(GPIO_BANK1, GPIO_PIN_TO_REGISTER(P8_3));

  EXPECT_EQ(1, GPIO_PIN_TO_BANK_NUM(ULED1));
  EXPECT_EQ(21, GPIO_PIN_TO_BIT_NUM(ULED1));
  EXPECT_EQ(GPIO_BANK1, GPIO_PIN_TO_REGISTER(ULED1));
}
