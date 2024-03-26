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

// TODO: The DATA_SECTION is incorrect. It causes the compiler
// to place this data in the text section, probably because it
// is page zero and there is not a linker command entry for the
// section names produced here.

#define SUPRUGLUE_DEFINE_SIZED(name, type1, thing1, type2, size)                                                       \
  SUPRUGLUE_DECLARE_SIZED(name, type1, thing1, type2, size);                                                           \
  _PRAGMA(DATA_SECTION(name, ".thread." #name))                                                                        \
  static SUPRUGLUE_SIZED_TYPENAME(name, type1, type2) name

#ifdef SUPRUGLUE_TEST32
#define _PRAGMA(x)
#else
#define _PRAGMA(x) _Pragma(#x)
#endif

#define _JOIN(thing, field) thing##field

#define WORDSZ sizeof(uint32_t)

#define NUM_WORDS(x) ((x) / WORDSZ)

#define SIZEOF_WORDS(x) NUM_WORDS(sizeof(x))

#define NUM_SYSEVTS ARCH_NUM_SYSEVTS

#define GPIO_NUM_REGISTERS 4

// test helpers, reset functions, etc
void SystemOnChipSetup(void);

// like a dynamic __delay_cycles
void SystemOnChipDelay(int32_t cycles);
void SystemOnChipSuspend(void);

void Shutdown(void);

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

    struct {
      unsigned LOW32 : 32;
      unsigned HIGH32 : 32;
    } CYCLES_32bit;
  };
};

// Pair of usage/stall cycle count
typedef struct _CPUCounter CPUCounter;

struct _CPUCounter {
  Timestamp run;
  Timestamp stall;
};

extern int __system_shutdown;

#ifdef __cplusplus
}
#endif

#endif // LIB_SOC_SOC_DEFS_H
