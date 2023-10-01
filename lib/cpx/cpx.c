// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "cpx.h"

// bogus
#include <unistd.h>

#include <assert.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

SystemConfig DefaultSystemConfig(void) {
  return (SystemConfig){.default_stack_size = DEFAULT_STACK_SIZE};
}
ThreadConfig DefaultThreadConfig(void) {
  return (ThreadConfig){.stack_size = DEFAULT_STACK_SIZE, .nice = DEFAULT_NICE};
}

int Init(System *sys, SystemConfig cfg) {
  memset(sys, 0, sizeof(*sys));
  sys->cfg = cfg;
  fprintf(stderr, "SIZEOF(VOID*)=%lu\n", sizeof(void *));
  return 0;
}

int Create(System *sys, Thread *thread, uint8_t *stack, ThreadFunc *func, void *arg, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.arg = arg;
  thread->state = STARTING;
  thread->stack = stack;
  thread->next = sys->runnable;
  sys->runnable = thread;
  return 0;
}

int Run(System *sys) {
  int volatile running = 1;
  while (running) {
    fprintf(stderr, "Enter while\n");
    running = 0;
    Thread *volatile run;

    for (run = sys->runnable; run != NULL; run = run->next) {
      sys->running = run;
      sys->run_stack_pos = (void *)&run;
      fprintf(stderr, "RunStackPos %p\n", sys->run_stack_pos);

      if (setjmp(sys->return_jump) == 0) {
        //  Start or resume a thread.
        switch (run->state) {
        case STARTING:
          run->state = RUNNING;
          fprintf(stderr, "Starting %p\n", sys->run_stack_pos);
          run->exec.call.func(run->exec.call.arg);
          run->state = FINISHED;
          break;
        case RUNNING:
          fprintf(stderr, "Continuing a running thread\n");
          longjmp(run->exec.run_jump, 1);
          break;
        case FINISHED:
          fprintf(stderr, "See a finished thread\n");
          break;
        }
        fprintf(stderr, "Looping around...\n");
        continue;
      }

      fprintf(stderr, "Yielded\n");
      running = 1;
      // Someone yielded, continue.
    }
    fprintf(stderr, "Finish while\n");
  }
  return 0;
}

void Yield(System *sys) {
  Thread *volatile not_used = sys->running;
  void *yield_stack_pos = (void *)&not_used;

  // yield_stack_pos++;
  fprintf(stderr, "YieldStackPos %p\n", yield_stack_pos);
  // Check for stack overflow.
  size_t size = (size_t)sys->run_stack_pos - (size_t)yield_stack_pos;
  fprintf(stderr, "To copy: %zx\n", size);
  if (sys->running->cfg.stack_size < size) {
    fprintf(stderr, "panic! %zx > %zx\n", sys->running->cfg.stack_size, size);
  }
  memcpy(sys->running->stack, yield_stack_pos, size);

  if (setjmp(sys->running->exec.run_jump) == 0) {
    fprintf(stderr, "Return to scheduler\n");
    longjmp(sys->return_jump, 1);
  }
  fprintf(stderr, "Arrive from scheduler\n");

  // yield_stack_pos is not volatile, wasn't restored.
  yield_stack_pos = (void *)&not_used;

  memcpy(yield_stack_pos, sys->running->stack, (size_t)sys->run_stack_pos - (size_t)yield_stack_pos);
  fprintf(stderr, "Return from yield\n");
}

ThreadID PID(System *sys) {
  return (ThreadID)sys->running;
}

#ifdef __cplusplus
}
#endif
