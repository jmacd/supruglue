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

System __system;

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

void threadListInit(ThreadList *list) {
  list->next = list;
  list->prev = list;
}

int threadListEmpty(ThreadList *head) {
  return head->next == head;
}

void threadListAdd(ThreadList *head, Thread *thread) {
  ThreadList *next = head->next;

  head->next = &thread->list;
  thread->list.next = next;
  thread->list.prev = head;
  ((Thread *)next)->list.prev = &thread->list;
}

Thread *threadListPopFront(ThreadList *head) {
  Thread *thread = (Thread *)head->next;

  head->next = thread->list.next;
  ((Thread *)thread->list.next)->list.prev = head;
  thread->list.prev = NULL;
  thread->list.next = NULL;
  return thread;
}

int Init(SystemConfig cfg) {
  System *sys = &__system;
  memset(sys, 0, sizeof(*sys));
  sys->cfg = cfg;
  threadListInit(&sys->runnable);
  return 0;
}

int Create(Thread *thread, ThreadFunc *func, Args args, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  memset(cfg.stack, 0, cfg.stack_size);
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.args = args;
  thread->state = STARTING;
  threadListAdd(__system.runnable.prev, thread);
  return 0;
}

int Run() {
  while (!threadListEmpty(&__system.runnable)) {
    Thread *volatile run = threadListPopFront(&__system.runnable);

    __system.current = run;
    __system.run_stack_pos = (void *)&run;

    if (setjmp(__system.return_jump) != 0) {
      threadListAdd(__system.runnable.prev, run);
      continue;
    }
    switch (run->state) {
    case STARTING:
      run->state = RUNNING;
      run->exec.call.func(TID(run), run->exec.call.args);
      run->state = FINISHED;
      break;
    case RUNNING:
      longjmp(run->exec.run_jump, 1);
      break;
    case FINISHED:
      break;
    }
  }
  return 0;
}

void Yield() {
  void *volatile unused;
  void *yield_stack = (void *)&unused;
  size_t size = (size_t)__system.run_stack_pos - (size_t)yield_stack;

  // Check for thread-stack overflow.
  if (size > __system.current->cfg.stack_size) {
    // TODO:
    assert(0);
  }
  memcpy(__system.current->cfg.stack, yield_stack, size);

  if (setjmp(__system.current->exec.run_jump) == 0) {
    longjmp(__system.return_jump, 1);
  }

  // size and yield_stack are not volatile, recompute:
  yield_stack = (void *)&unused;
  size = (size_t)__system.run_stack_pos - (size_t)yield_stack;

  memcpy(yield_stack, __system.current->cfg.stack, size);
}

#ifdef __cplusplus
}
#endif
