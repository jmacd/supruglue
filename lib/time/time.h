// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_TIME_TIME_H
#define LIB_TIME_TIME_H

#include <stdint.h>

#include "lib/coroutine/coroutine.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t cycle_t;
typedef uint64_t duration_t;

void Sleep(pru_dur_t d);

#ifdef __cplusplus
}
#endif

#endif // LIB_TIME_TIME_H
