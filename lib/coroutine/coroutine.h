// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_COROUTINE_COROUTINE_H
#define LIB_COROUTINE_COROUTINE_H

#include <setjmp.h>

#include "lib/args/args.h"
#include "lib/log/journal/journal.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _System;
struct _SystemConfig;

typedef struct _System       System;
typedef struct _SystemConfig SystemConfig;

struct _SystemConfig {
  int32_t shutdown;
};

struct _System {
  SystemConfig cfg;
  jmp_buf      return_jump;
  void        *run_stack_pos;
  Thread      *allthreads;
  Journal      journal;
};

extern System __system;

SystemConfig NewSystemConfig(void);

int Init(SystemConfig cfg);

int Create(Thread *thread, ThreadFunc *func, Args args, const char *name, size_t stack_size);

int Run(void);

void Shutdown(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_COROUTINE_COROUTINE_H
