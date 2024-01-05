// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/time/clock.h"

#pragma DATA_SECTION(__system_shutdown, ".resource_table")
System __system;

SystemConfig NewSystemConfig(void) {
  return (SystemConfig){
      .export_interval = 10 * TIME_SECOND,
  };
}

int Init(SystemConfig cfg) {
  System *sys = &__system;
  memset(sys, 0, sizeof(*sys));
  sys->cfg = cfg;
  SystemOnChipSetup();
  ThreadListInit(&__system_runnable);
  JournalInit(&sys->journal);
  __system_shutdown = 0;
  return 0;
}

int Create(Thread *thread, ThreadFunc *func, Args args, const char *name, size_t stack_size) {
  memset(thread, 0, sizeof(*thread));
  thread->name = name;
  thread->stack_size = stack_size;
  thread->exec.call.func = func;
  thread->exec.call.args = args;
  thread->state = TS_STARTING;
  thread->allthreads = __system.allthreads;
  __system.allthreads = thread;
  ThreadListPushBack(&__system_runnable, thread);
  return 0;
}

int __run(void) {
  while (!__system_shutdown && !ThreadListEmpty(&__system_runnable)) {
    Thread *volatile run = ThreadListPopFront(&__system_runnable);

    __system.run_stack_pos = (void *)&run;

    TimedSwitch();

    __system_current = run;

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
      default:
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
    PRULOG_2u32(FATAL, "stack overflow: %u exceeds %u", size, __system_current->stack_size);
    longjmp(__system.return_jump, JC_OVERFLOW);
  }

  memcpy(__system_current->stack, yield_stack, size);

  // Set return point.
  if (setjmp(__system_current->exec.run_jump) == JC_SETJUMP) {
    // Return control.
    longjmp(__system.return_jump, jc);
  }
  // Resume control.

  // size and yield_stack are not volatile, but they have to be
  // recomputed for other reasons.  TODO: not clear why--is the stack
  // copy off in size?
  yield_stack = (void *)&unused;
  size = (size_t)__system.run_stack_pos - (size_t)yield_stack;

  memcpy(yield_stack, __system_current->stack, size);
}

int Run(void) {
  __system_yield = &yieldInternal;

  // set current for TimedSwitch() to avoid extra branches
  __system_current = ThreadListFront(&__system_runnable);
  TimeStart();

  int err = __run();
  __system_yield = NULL;
  __system_current = NULL;
  return err;
}
