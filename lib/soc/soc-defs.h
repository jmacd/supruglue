// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SOC_SOC_DEFS_H
#define LIB_SOC_SOC_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SUPRUGLUE_SIZED_TYPENAME(name, type1, type2) name##_##type1##_with_##type2

#define SUPRUGLUE_DECLARE_SIZED(name, type1, thing1, type2, size)                                                      \
  typedef struct _##name##_##type1##_with_##type2 SUPRUGLUE_SIZED_TYPENAME(name, type1, type2);                        \
  struct _##name##_##type1##_with_##type2 {                                                                            \
    type1 thing1;                                                                                                      \
    type2 space[size];                                                                                                 \
  }

#define SUPRUGLUE_DEFINE_SIZED(name, type1, thing1, type2, size)                                                       \
  SUPRUGLUE_DECLARE_SIZED(name, type1, thing1, type2, size);                                                           \
  static SUPRUGLUE_SIZED_TYPENAME(name, type1, type2) name

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

// Timestamp is out of place. Where should it go?
typedef struct _Timestamp Timestamp;

struct _Timestamp {
  union {
    volatile uint64_t NANOS;

    volatile struct {
      unsigned HIGH : 32;
      unsigned LOW : 32;
    } NANOS_bit;
  };
};

#ifdef __cplusplus
}
#endif

#endif // LIB_SOC_SOC_DEFS_H
