// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CPX_CPX_H
#define LIB_CPX_CPX_H

#include <setjmp.h>

#include "lib/args/args.h"
#include "lib/thread/thread.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _System;
struct _SystemConfig;

typedef struct _System       System;
typedef struct _SystemConfig SystemConfig;

enum JumpCode {
  JC_SETJUMP = 0,
  JC_SUSPEND = 1,
  JC_RESUME = 2,
  JC_OVERFLOW = 3,
  JC_INTERNAL = 4,
  JC_BLOCKED = 5,
  JC_UNBLOCKED = 6,
};

typedef enum JumpCode JumpCode;

struct _SystemConfig {
  int32_t unused;
};

struct _System {
  SystemConfig cfg;
  jmp_buf      return_jump;
  void        *run_stack_pos;
  Thread      *current;
  ThreadList   runnable;
};

extern System __system;

SystemConfig NewSystemConfig(void);

int Init(SystemConfig cfg);

int Create(Thread *thread, ThreadFunc *func, Args args, ThreadConfig cfg);

int Run(void);

void Yield(void);

void yieldInternal(JumpCode jc);

#ifdef __cplusplus
}
#endif

#endif // LIB_CPX_CPX_H
