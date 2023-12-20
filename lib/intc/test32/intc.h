// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_INTC_TEST32_INTC_H
#define LIB_INTC_TEST32_INTC_H

#include "lib/intc/intc-defs.h"
#include "lib/soc/soc.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TestInterruptController TestInterruptController;

struct _InterruptController {
  TestInterruptController *test;
};

void RaiseInterrupt(uint8_t evt);

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_TEST32_INTC_H
