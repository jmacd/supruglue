// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_THREAD_THREAD_H
#define LIB_THREAD_THREAD_H

#include "lib/args/args.h"
#include "lib/list/list.h"
#include "lib/soc/soc.h"
#include <setjmp.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uintptr_t ThreadID;

struct _Thread;

typedef struct _Thread Thread;

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

struct _Thread {
  ThreadList  list;
  const char *name;
  int32_t     stack_size;
  ThreadState state;

  union {
    jmp_buf run_jump;
    struct {
      ThreadFunc *func;
      Args        args;
    } call;
  } exec;

  uint8_t stack[0];
};

SUPRUGLUE_DEFINE_LIST_INLINE(ThreadList, Thread, list);

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

#define SUPRUGLUE_DEFINE_THREAD(nam, stack_size) SUPRUGLUE_DEFINE_SIZED(nam, Thread, thread, uint8_t, stack_size)

#ifdef __cplusplus
}
#endif

#endif // LIB_THREAD_THREAD_H
