// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
#define SUPRUGLUE_TEST_SUPRUGLUE_SOC_H

#include <stdint.h>

#include "supruglue/soc.h"

#define WORDSZ sizeof(uint32_t)

typedef struct {
} supruglue_test_system;

extern supruglue_test_system _SOC;

void Test_Panic(const char *str);

#endif // SUPRUGLUE_TEST_SUPRUGLUE_SOC_H
