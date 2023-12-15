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

// @@@ Still sorta a placeholder
void BlockOnSystemEvent(InterruptController *controller, uint8_t evt);

void ClearSystemEvent(uint8_t evt);

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_INTC_DEFS_H
