// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifdef __cplusplus
extern "C" {
#endif

#include "cpx.h"

#include <assert.h>
#include <stdlib.h>

SystemConfig DefaultSystemConfig() {
  return (SystemConfig){.default_stack_size = DEFAULT_STACK_SIZE};
}
ThreadConfig DefaultThreadConfig() {
  return (ThreadConfig){.stack_size = DEFAULT_STACK_SIZE, .nice = DEFAULT_NICE};
}

int Init(System *sys, SystemConfig cfg) {
  memset(sys, 0, sizeof(*sys));
  sys->cfg = cfg;
  return 0;
}

int Create(System *sys, Thread *thread, uint8_t *stack, ThreadFunc *func, void *arg, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.arg = arg;
  thread->next = sys->runnable;
  thread->state = STARTING;
  thread->stack = stack;
  sys->runnable = thread;
  return 0;
}

int Run(System *sys) {
  Thread *volatile run;

  for (run = sys->runnable; run != NULL; run = run->next) {
    sys->run_stack_pos = (void *)&run;

    if (setjmp(sys->return_jump) == 0) {
      // Start or resume a thread.
      switch (run->state) {
      case STARTING:
        printf("Starting\n");
        run->exec.call.func(run->exec.call.arg);
        // @@@ Stop the thread, remove from runnable
        break;
      case RUNNING:
        printf("Continued\n");
        longjmp(run->exec.run_jump, 1);
        break;
      }
      continue;
    }

    printf("Yielded\n");
    // Someone yielded, continue.
  }
  return 0;
}

void Yield(System *sys) {
  void *volatile not_used;
  void *yield_stack_pos = (void *)&not_used;
  yield_stack_pos++;
  printf("Hii %p %p %u\n", yield_stack_pos, sys->run_stack_pos, sys->run_stack_pos - yield_stack_pos);

  // Check for stack overflow.
  memcpy(sys->runnable->stack, yield_stack_pos, sys->run_stack_pos - yield_stack_pos);

  if (setjmp(sys->runnable->exec.run_jump) == 0) {
    printf("Bye\n");
    longjmp(sys->return_jump, 1);
  }

  // yield_stack_pos is not volatile, wasn't restored.
  yield_stack_pos = (void *)&not_used;
  memcpy(yield_stack_pos, sys->runnable->stack, sys->run_stack_pos - yield_stack_pos);
}

ThreadID PID(System *sys) {
  return (ThreadID)sys->runnable;
}

#ifdef __cplusplus
}
#endif
