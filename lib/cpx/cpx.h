// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_CPX_CPX_H
#define LIB_CPX_CPX_H

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lib/cpx/args.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_NICE 0
#define LOG_CHANNEL_ENTRIES 4U

struct _Thread;
struct _ThreadConfig;
struct _System;
struct _SystemConfig;

typedef struct _Thread       Thread;
typedef struct _ThreadConfig ThreadConfig;
typedef struct _ThreadList   ThreadList;
typedef struct _System       System;
typedef struct _SystemConfig SystemConfig;
typedef struct _Channel      Channel;
typedef struct _LogChannel   LogChannel;
typedef struct _LogEntry     LogEntry;

typedef uintptr_t ThreadID;

typedef void(ThreadFunc)(ThreadID thid, Args args);

enum ThreadState {
  STARTING = 1, // Use exec.call.func(exec.call.arg)
  RUNNING = 2,  // Use longjump(exec.run_jump)
  FINISHED = 3, // Nothing to do
};

enum JumpCode {
  SETJUMP = 0,
  CONTINUING = 1,
  OVERFLOW = 2,
};

typedef enum ThreadState ThreadState;
typedef enum JumpCode    JumpCode;

struct _ThreadConfig {
  uint8_t    *stack;
  int32_t     stack_size;
  uint8_t     nice;
  const char *name;
};

struct _ThreadList {
  ThreadList *next;
  ThreadList *prev;
};

struct _Thread {
  ThreadList   list;
  ThreadConfig cfg;
  ThreadState  state;

  union {
    jmp_buf run_jump;
    struct {
      ThreadFunc *func;
      Args        args;
    } call;
  } exec;
};

struct _SystemConfig {
  size_t unused;
};

struct _LogEntry {
  ThreadID    tid;
  const char *msg;
  int32_t     arg1;
  int32_t     arg2;
};

struct _Channel {
  uint32_t head;
  uint32_t tail;
  uint8_t  buffer[0];
};

struct _LogChannel {
  Channel base;
  uint8_t space[sizeof(LogEntry) * LOG_CHANNEL_ENTRIES];
};

struct _System {
  SystemConfig cfg;
  LogChannel   log;

  jmp_buf return_jump;
  void   *run_stack_pos;

  ThreadList runnable;
  Thread    *current;
};

extern System __system;

SystemConfig NewSystemConfig(void);
ThreadConfig NewThreadConfig(const char *name, uint8_t *stack, size_t stack_size);

int Init(SystemConfig cfg);

int Create(Thread *thread, ThreadFunc *func, Args args, ThreadConfig cfg);

int Run();

void Yield();

int  channelEmpty(Channel *ch);
void channelRead(Channel *ch, int32_t ch_size, void *data, size_t data_size);
void channelWrite(Channel *ch, int32_t ch_size, void *data, size_t data_size);

inline ThreadID TID(Thread *th) {
  return (ThreadID)th;
}

#ifdef __cplusplus
}
#endif

#endif // LIB_CPX_CPX_H
