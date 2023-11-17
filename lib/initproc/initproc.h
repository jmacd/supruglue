// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_INITPROC_INITPROC_H
#define LIB_INITPROC_INITPROC_H

#include "lib/args/args.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

// Thread function that reads input messages from the ARM host, at a
// minimum handles RPMsg interrupts.

void InitProcess(ThreadID thid, Args args);

#ifdef __cplusplus
}
#endif

#endif // LIB_INITPROC_INITPROC_H
