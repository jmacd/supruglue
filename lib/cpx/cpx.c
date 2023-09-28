// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifdef __cplusplus
extern "C" {
#endif

#include "cpx.h"

#include <assert.h>
#include <stdlib.h>

SystemConfig DefaultSystemConfig() { return (SystemConfig){.defaultStackSize = DEFAULT_STACK_SIZE}; }
ThreadConfig DefaultThreadConfig() { return (ThreadConfig){.stackSize = DEFAULT_STACK_SIZE, .nice = DEFAULT_NICE}; }

int Init(System *sys, SystemConfig cfg) {
  memset(sys, 0, sizeof(*sys));
  sys->cfg = cfg;
  return 0;
}

int Create(System *sys, Thread *thread, ThreadFunc *func, void *arg, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.arg = arg;
  thread->next = sys->runnable;
  thread->state = STARTING;
  sys->runnable = thread;
  return 0;
}

int Run(System *sys) {
  for (volatile Thread *run = sys->runnable; run != NULL; run = run->next) {
    // HERE: save current, setjmp
    int err = setjump(sys->return_jump);
    if (err != 0) {
      // @@@
      return err;
    }

    switch (run->state) {
    case STARTING:
      run->exec.call.func(run->exec.call.arg);
      break;
    case RUNNING:
      longjmp(run->exec.call.run_jump, 1);
    }
  }
  return 0;
}

void Yield() {
  // @@@ Got it, so we need a global at this point, the place to save the
  // jump buf is per thread can't be a global, so need a global thread pointer.
  setjump(XXX->run_buf);
}

ProcessID Pid() { return 0; }

#ifdef __cplusplus
}
#endif
