// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_INTC_AM335X_INTC_H
#define LIB_INTC_AM335X_INTC_H

#include "lib/intc/intc-defs.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _InterruptController {
  // TODO mostly placeholder
  ThreadList blocked;
};

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_AM335X_INTC_H
