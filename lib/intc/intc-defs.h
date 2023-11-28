// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_INTC_INTC_DEFS_H
#define LIB_INTC_INTC_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _InterruptController InterruptController;

extern InterruptController __controller;

void ControllerInit(InterruptController *controller);

void ServiceInterrupts(InterruptController *controller);

// TODO: This is a placeholder.
void BlockOnHost0(InterruptController *controller);

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_INTC_DEFS_H
