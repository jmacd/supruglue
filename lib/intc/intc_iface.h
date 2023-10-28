// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_INTC_INTC_IFACE_H
#define LIB_INTC_INTC_IFACE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _InterruptController InterruptController;

extern InterruptController __interrupts;

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_INTC_IFACE_H
