#include "include/gpio.h"
#include "gtest/gtest.h"

const uint32_t allbits = 0xffffffff;
const uint32_t halfbits = 0x0ff00ff0;

supruglue_gpio *allGPIOs[GPIO_NUM_BANKS] = {
    GPIO_BANK0,
    GPIO_BANK1,
    GPIO_BANK2,
    GPIO_BANK3,
};

TEST(GpioTest, Output) {
  for (int i = 0; i < GPIO_NUM_BANKS; i++) {
    supruglue_gpio *gpio = allGPIOs[i];

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
