// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_AM335X_TIME_H
#define LIB_TIME_AM335X_TIME_H

#include "lib/time/time-defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PRU_MHZ 2e8

uint32_t RawCycleCounter(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_AM335X_TIME_H
