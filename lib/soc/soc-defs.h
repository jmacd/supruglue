// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SOC_SOC_DEFS_H
#define LIB_SOC_SOC_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SUPRUGLUE_DEFINE_THINGS(name, type1, thing1, type2, thing2, size)                                              \
  typedef struct _##name##_##thing1##_with_##thing2 name##_##thing1##_with_##thing2;                                   \
  struct _##name##_##thing1##_with_##thing2 {                                                                          \
    type1 thing1;                                                                                                      \
    type2 thing2[size];                                                                                                \
  };                                                                                                                   \
  static name##_##thing1##_with_##thing2 name

#define _JOIN(thing, field) thing##field

#define WORDSZ sizeof(uint32_t)

#define NUM_WORDS(x) ((x) / WORDSZ)

#define SIZEOF_WORDS(x) NUM_WORDS(sizeof(x))

#define GPIO_NUM_REGISTERS 4

// test helpers, reset functions, etc
void SystemOnChipSetup(void);
void SystemOnChipTeardown(void);

// maps into e.g., dynamic __delay_cycles
void SystemOnChipDelay(int32_t cycles);

#ifdef __cplusplus
}
#endif

#endif // LIB_SOC_SOC_DEFS_H
