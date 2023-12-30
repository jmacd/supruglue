// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_AM335X_TIME_H
#define LIB_TIME_AM335X_TIME_H

#include "lib/time/clock-defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TIME_SECOND 200000000

extern Timestamp __clock;

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_AM335X_TIME_H
