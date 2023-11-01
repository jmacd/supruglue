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

ThreadID TID(Thread *th);

#ifdef __cplusplus
}
#endif

#endif // LIB_THREAD_THREAD_H
