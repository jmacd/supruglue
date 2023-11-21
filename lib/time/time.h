// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_H
#define LIB_TIME_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/time/time-defs.h"

#if defined(SUPRUGLUE_AM335X)
#include "lib/time/am335x/time-am335x.h"
#elif defined(SUPRUGLUE_TEST32)
#include "lib/time/test32/time-test32.h"
#endif

int Sleep(Duration d);

int ClockInit(Clock *clock);

void TimeAdd(Clock *clock, Duration dur);

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_H
