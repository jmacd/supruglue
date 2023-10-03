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

SystemConfig DefaultSystemConfig() {
  return (SystemConfig){};
}

ThreadConfig DefaultThreadConfig(uint8_t *stack, size_t stack_size) {
  return (ThreadConfig){
      .nice = DEFAULT_NICE,
      .stack = stack,
      .stack_size = stack_size,
  };
}

int Init(System *sys, SystemConfig cfg) {
  memset(sys, 0, sizeof(*sys));
  sys->cfg = cfg;
  fprintf(stderr, "SIZEOF(VOID*)=%lu\n", sizeof(void *));
  return 0;
}

int Create(System *sys, Thread *thread, ThreadFunc *func, const char *args, size_t argsize, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  memset(cfg.stack, 0, cfg.stack_size);
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.args = args;
  thread->exec.call.argsize = argsize;
  fprintf(stderr, "Note func is %p\n", thread->exec.call.func);

  thread->state = STARTING;
  thread->next = sys->runnable;
  sys->runnable = thread;
  return 0;
}

int Run(System *volatile sys) {
  int volatile running = 1;
  while (running) {
    fprintf(stderr, "Enter while\n");
    running = 0;
    Thread *volatile run;

    for (run = sys->runnable; run != NULL; run = run->next) {
      fprintf(stderr, "Pre-Run is %p\n", run);
      sys->running = run;
      sys->run_stack_pos = (void *)&run;
      fprintf(stderr, "RunStackPos %p\n", sys->run_stack_pos);

      if (setjmp(sys->return_jump) == 0) {
        //  Start or resume a thread.
        switch (run->state) {
        case STARTING:
          run->state = RUNNING;
          fprintf(stderr, "Starting %p note %p\n", sys->run_stack_pos, run->exec.call.func);
          run->exec.call.func(sys, TID(run), run->exec.call.args, run->exec.call.argsize);

          sys->run_stack_pos = (void *)&run;

          fprintf(stderr, "Finishing %p\n", sys->run_stack_pos);
          fprintf(stderr, "Post-Run is %p\n", run);
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
      sys->running = NULL;
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
  size_t volatile size = (size_t)sys->run_stack_pos - (size_t)yield_stack_pos;
  fprintf(stderr, "To copy: %zx\n", size);
  if (sys->running->cfg.stack_size < size) {
    fprintf(stderr, "panic! %zx > %zx\n", sys->running->cfg.stack_size, size);
  }
  memcpy(sys->running->cfg.stack, yield_stack_pos, size);

  if (setjmp(sys->running->exec.run_jump) == 0) {
    fprintf(stderr, "Return to scheduler\n");
    longjmp(sys->return_jump, 1);
  }
  // yield_stack_pos is not volatile, wasn't restored.
  yield_stack_pos = (void *)&not_used;

  fprintf(stderr, "Arrive from scheduler sys: %p\n", sys);
  fprintf(stderr, "Arrive from scheduler A: %zx\n", (size_t)yield_stack_pos);
  fprintf(stderr, "Arrive from scheduler B: %zx\n", (size_t)sys->run_stack_pos);
  fprintf(stderr, "Arrive from scheduler size: %zx\n", size);

  memcpy(yield_stack_pos, sys->running->cfg.stack, size);
  fprintf(stderr, "Return from yield\n");
}

#ifdef __cplusplus
}
#endif
