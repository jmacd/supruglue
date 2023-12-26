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

#define NUM_SYSEVTS ARCH_NUM_SYSEVTS

#define GPIO_NUM_REGISTERS 4

// test helpers, reset functions, etc
void SystemOnChipSetup(void);
int  SystemOnChipIsShutdown(void);

// like a dynamic __delay_cycles
void SystemOnChipDelay(int32_t cycles);
void SystemOnChipSuspend(void);

// Counts 5ns cycles.
typedef struct _Timestamp Timestamp;

struct _Timestamp {
  union {
    uint64_t CYCLES;

    struct {
      unsigned LOW : 16;
      unsigned MIDDLE : 16;
      unsigned HIGH : 32;
    } CYCLES_bit;
  };
};

// Pair of usage/stall cycle count
typedef struct _CPUCounter CPUCounter;

struct _CPUCounter {
  Timestamp run;
  Timestamp stall;
};

#ifdef __cplusplus
}
#endif

#endif // LIB_SOC_SOC_DEFS_H
