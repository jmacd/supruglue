// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_DEFS_H
#define LIB_TIME_TIME_DEFS_H

#include "lib/soc/soc.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

// Called by ClockInit().
void PeriodInit(void);

// Sleepers, used by clock process.
// extern ThreadList __asleep;

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_DEFS_H
