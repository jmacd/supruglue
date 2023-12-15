// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_INTC_INTC_DEFS_H
#define LIB_INTC_INTC_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _InterruptController InterruptController;

typedef void(InterruptHandler)(void);

extern InterruptController __controller;

void ControllerInit(void);
void InterruptHandlerInit(uint8_t evt, InterruptHandler *handler);
void ServiceInterrupts(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_INTC_INTC_DEFS_H
