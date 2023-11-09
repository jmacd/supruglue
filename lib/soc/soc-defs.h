// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SOC_SOC_DEFS_H
#define LIB_SOC_SOC_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _JOIN(thing, field) thing##field

#define WORDSZ sizeof(uint32_t)

#define NUM_WORDS(x) ((x) / WORDSZ)

#define SIZEOF_WORDS(x) NUM_WORDS(sizeof(x))

#define GPIO_NUM_REGISTERS 4

static inline void SystemOnChipSetup();
static inline void SystemOnChipTeardown();

static inline void SystemOnChipDelay(int32_t cycles);

#ifdef __cplusplus
}
#endif

#endif // LIB_SOC_SOC_DEFS_H
