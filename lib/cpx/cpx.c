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

int Create(System *sys, Thread *thread, ThreadFunc func, void *arg, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.arg = arg;
  thread->next = sys->runnable;
  sys->runnable = thread;
  return 0;
}

int Run(System *sys) {
  for (Thread *run = sys->runnable; run != NULL; run = run->next) {
    // HERE: save current, setjmp
    run->exec.call.func(run->exec.call.arg);
  }
  return 0;
}

void Yield() {
  // HERE: stop current, longjump, etc, set runnable, etc
}

#ifdef __cplusplus
}
#endif
