// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <setjmp.h>
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

  ThreadListInit(&sys->runnable);
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
  ThreadListPushBack(&__system.runnable, thread);
  return 0;
}

int Run(void) {
  while (!ThreadListEmpty(&__system.runnable)) {
    Thread *volatile run = ThreadListPopFront(&__system.runnable);

    __system.current = run;
    __system.run_stack_pos = (void *)&run;

    switch (setjmp(__system.return_jump)) {
    case JC_SETJUMP:
      //  The return jump was prepared, break to the second switch with
      //  the current runnable thread.
      break;
    case JC_SUSPEND:
      //  Running thread yielded.
      ThreadListPushBack(&__system.runnable, run);
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
      assert(0);
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
  if (size > __system.current->cfg.stack_size) {
    JournalWrite(&__system.journal, TID(__system.current), "stack overflow: %u exceeds %u", size,
                 __system.current->cfg.stack_size);
    longjmp(__system.return_jump, JC_OVERFLOW);
  }
  memcpy(__system.current->cfg.stack, yield_stack, size);

  switch (setjmp(__system.current->exec.run_jump)) {
  case JC_SETJUMP:
    longjmp(__system.return_jump, jc);
  case JC_RESUME:
    break;
  default:
    assert(0);
  }

  //  size and yield_stack are not volatile, recompute:
  yield_stack = (void *)&unused;
  size = (size_t)__system.run_stack_pos - (size_t)yield_stack;

  memcpy(yield_stack, __system.current->cfg.stack, size);
}

void Yield(void) {
  yieldInternal(JC_SUSPEND);
}

void YieldBlocked(void) {
  yieldInternal(JC_BLOCKED);
}
