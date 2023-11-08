// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_SOC_SOC_DEFS_H
#define LIB_SOC_SOC_DEFS_H

#define _JOIN(thing, field) thing##field

#define WORDSZ sizeof(uint32_t)

#define NUM_WORDS(x) ((x) / WORDSZ)

#define SIZEOF_WORDS(x) NUM_WORDS(sizeof(x))

#endif // LIB_SOC_SOC_DEFS_H
