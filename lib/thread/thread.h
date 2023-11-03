// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_THREAD_THREAD_H
#define LIB_THREAD_THREAD_H

#include "lib/args/args.h"
#include "lib/list/list.h"
#include <setjmp.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uintptr_t ThreadID;

struct _Thread;
struct _ThreadConfig;

typedef struct _Thread       Thread;
typedef struct _ThreadConfig ThreadConfig;

SUPRUGLUE_DECLARE_LIST(ThreadList, Thread);

typedef void(ThreadFunc)(ThreadID thid, Args args);

enum ThreadState {
  TS_STARTING = 1, // Use exec.call.func(exec.call.arg)
  TS_RUNNING = 2,  // Use longjump(exec.run_jump)
  TS_FINISHED = 3, // Nothing to do
};

typedef enum ThreadState ThreadState;

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

struct _ThreadConfig {
  const char *name;
  uint8_t    *stack;
  int32_t     stack_size;
  uint8_t     nice;
};

struct _Thread {
  ThreadConfig cfg;
  ThreadList   list;
  ThreadState  state;

  union {
    jmp_buf run_jump;
    struct {
      ThreadFunc *func;
      Args        args;
    } call;
  } exec;
};

ThreadConfig NewThreadConfig(const char *name, uint8_t *stack, size_t stack_size);

typedef void(SystemYield)(JumpCode jc);

extern SystemYield *__system_yield;
extern Thread      *__system_current;
extern ThreadList   __system_runnable;

static inline ThreadID TID(Thread *th) {
  return (ThreadID)th;
}

static inline void Yield(void) {
  (*__system_yield)(JC_SUSPEND);
}

static inline void YieldBlocked(void) {
  (*__system_yield)(JC_BLOCKED);
}

#ifdef __cplusplus
}
#endif

#endif // LIB_THREAD_THREAD_H
