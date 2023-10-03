// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CPX_CPX_H
#define LIB_CPX_CPX_H

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_NICE 0

struct _Thread;
struct _ThreadConfig;
struct _System;
struct _SystemConfig;

typedef struct _Thread Thread;
typedef struct _ThreadConfig ThreadConfig;
typedef struct _System System;
typedef struct _SystemConfig SystemConfig;

typedef uintptr_t ThreadID;

typedef void(ThreadFunc)(System *sys, ThreadID thid, const char *args, size_t argsize);

enum ThreadState {
  STARTING, // Use exec.call.func(exec.call.arg)
  RUNNING,  // Use longjump(exec.run_jump)
  FINISHED, // Nothing to do
};

typedef enum ThreadState ThreadState;

struct _ThreadConfig {
  uint8_t *stack;
  size_t stack_size;
  uint32_t nice;
};

struct _Thread {
  Thread *next;
  ThreadConfig cfg;
  ThreadState state;

  // exec is a jump buffer or the initial function/arg
  union {
    jmp_buf run_jump;
    struct {
      ThreadFunc *func;
      const char *args;
      size_t argsize;
    } call;
  } exec;
};

struct _SystemConfig {
  size_t unused;
};

struct _System {
  SystemConfig cfg;
  jmp_buf return_jump;
  void *run_stack_pos;
  Thread *runnable;
  Thread *running;
};

SystemConfig DefaultSystemConfig(void);
ThreadConfig DefaultThreadConfig(uint8_t *stack, size_t stack_size);

int Init(System *sys, SystemConfig cfg);

int Create(System *sys, Thread *thread, ThreadFunc *func, const char *args, size_t argsize, ThreadConfig cfg);

int Run(System *sys);

void Yield(System *sys);

inline ThreadID TID(Thread *th) {
  return (ThreadID)th;
}

#ifdef __cplusplus
}
#endif

#endif // LIB_CPX_CPX_H
