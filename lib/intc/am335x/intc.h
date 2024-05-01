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
  // Shrug this should use generated code so that only the enabled
  // interrupts occupy space, and the lookup is O(1).  For now waste
  // some space.
  InterruptHandler *handler[NUM_SYSEVTS];
  Args              args[NUM_SYSEVTS];
};

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_AM335X_INTC_H
