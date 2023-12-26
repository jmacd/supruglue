// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/time/clock.h"

System __system;

SystemConfig NewSystemConfig(void) {
  return (SystemConfig){
      .unused = 0,
  };
}

int Init(SystemConfig cfg) {
  System *sys = &__system;
  memset(sys, 0, sizeof(*sys));
  sys->cfg = cfg;
  SystemOnChipSetup();
  ThreadListInit(&__system_runnable);
  JournalInit(&sys->journal);
  return 0;
}

int Create(Thread *thread, ThreadFunc *func, Args args, const char *name, size_t stack_size) {
  memset(thread, 0, sizeof(*thread));
  thread->name = name;
  thread->stack_size = stack_size;
  thread->exec.call.func = func;
  thread->exec.call.args = args;
  thread->state = TS_STARTING;
  ThreadListPushBack(&__system_runnable, thread);
  return 0;
}

CPUCounter *running;

int __run(void) {

  while (!SystemOnChipIsShutdown()) {
    Thread *volatile run = ThreadListPopFront(&__system_runnable);

    __system.run_stack_pos = (void *)&run;
    __system_current = run;

    // TimeSwitch();

    // Control flow:
    // 1. setjmp() returns JC_SETJUMP
    // 2. switch (run->state) starts, resumes, or exits a thread
    // 3. return here for non-JC_SETJUMP codes
    switch (setjmp(__system.return_jump)) {
    case JC_SUSPEND:
      //  Running thread yielded.
      ThreadListPushBack(&__system_runnable, run);
      continue;
    case JC_BLOCKED:
      //  Running thread became blocked by another.
      continue;
    case JC_OVERFLOW:
    case JC_INTERNAL:
      // Running thread had an error.
      continue;
    default: // e.g., JC_SETJUMP
      switch (run->state) {
      case TS_STARTING:
        //  Run a new thread.
        run->state = TS_RUNNING;
        run->exec.call.func(TID(run), run->exec.call.args);
        continue;
      case TS_RUNNING:
        // Re-enter the yield call.
        longjmp(run->exec.run_jump, JC_RESUME);
        // Function call returned
        continue;
      }
    }
  }
  return 0;
}

void yieldInternal(JumpCode jc) {
  void *volatile unused;
  void   *yield_stack = (void *)&unused;
  int32_t size = (int32_t)((size_t)__system.run_stack_pos - (size_t)yield_stack);

  // Check for thread-stack overflow.
  if (size > __system_current->stack_size) {
    PRULOG_2U(FATAL, "stack overflow: %u exceeds %u", size, __system_current->stack_size);
    longjmp(__system.return_jump, JC_OVERFLOW);
  }

  memcpy(__system_current->stack, yield_stack, size);

  switch (setjmp(__system_current->exec.run_jump)) {
  case JC_SETJUMP:
    longjmp(__system.return_jump, jc);
  case JC_RESUME:
    break;
  default:
    // assert(0);
    break;
  }

  //  size and yield_stack are not volatile, recompute:
  yield_stack = (void *)&unused;
  size = (size_t)__system.run_stack_pos - (size_t)yield_stack;

  memcpy(yield_stack, __system_current->stack, size);
}

int Run(void) {
  __system_yield = &yieldInternal;

  TimeStart();

  int err = __run();
  __system_yield = NULL;
  __system_current = NULL;
  return err;
}
