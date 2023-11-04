// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/cpx/cpx.h"

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

  ThreadListInit(&__system_runnable);
  JournalInit(&sys->journal);
  return 0;
}

int Create(Thread *thread, ThreadFunc *func, Args args, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  memset(cfg.stack, 0, cfg.stack_size);
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.args = args;
  thread->state = TS_STARTING;
  ThreadListPushBack(&__system_runnable, thread);
  return 0;
}

int __run(void) {
  while (!ThreadListEmpty(&__system_runnable)) {
    Thread *volatile run = ThreadListPopFront(&__system_runnable);

    __system_current = run;
    __system.run_stack_pos = (void *)&run;

    switch (setjmp(__system.return_jump)) {
    case JC_SETJUMP:
      //  The return jump was prepared, break to the second switch with
      //  the current runnable thread.
      break;
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
      run->state = TS_FINISHED;
      break;
    default:
      // assert(0);
      break;
    }
    switch (run->state) {
    case TS_STARTING:
      //  Run a new thread.
      run->state = TS_RUNNING;
      run->exec.call.func(TID(run), run->exec.call.args);
      run->state = TS_FINISHED;
      break;
    case TS_RUNNING:
      //  Re-enter the yield call.
      longjmp(run->exec.run_jump, JC_RESUME);
      break;
      // Function call returned
    case TS_FINISHED:
      //  Error cases exit here.
      break;
    }
  }
  return 0;
}

void yieldInternal(JumpCode jc) {
  void *volatile unused;
  void   *yield_stack = (void *)&unused;
  int32_t size = (int32_t)((size_t)__system.run_stack_pos - (size_t)yield_stack);

  // Check for thread-stack overflow.
  if (size > __system_current->cfg.stack_size) {
    PRULOG_2U(FATAL, "stack overflow: %u exceeds %u", size, __system_current->cfg.stack_size);
    longjmp(__system.return_jump, JC_OVERFLOW);
  }
  memcpy(__system_current->cfg.stack, yield_stack, size);

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

  memcpy(yield_stack, __system_current->cfg.stack, size);
}

int Run(void) {
  __system_yield = &yieldInternal;
  int err = __run();
  __system_yield = NULL;
  __system_current = NULL;
  return err;
}
