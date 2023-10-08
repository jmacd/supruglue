// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdlib.h>

#include "lib/cpx/cpx.h"

#ifdef __cplusplus
extern "C" {
#endif

System __system;

SystemConfig NewSystemConfig() {
  return (SystemConfig){
      .log_flags = CF_NONE,
  };
}

ThreadConfig NewThreadConfig(const char *name, uint8_t *stack, size_t stack_size) {
  return (ThreadConfig){
      .nice = DEFAULT_NICE,
      .stack = stack,
      .stack_size = stack_size,
      .name = name,
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

  channelInit(&sys->log.ch, cfg.log_flags);

  return 0;
}

int Create(Thread *thread, ThreadFunc *func, Args args, ThreadConfig cfg) {
  memset(thread, 0, sizeof(*thread));
  memset(cfg.stack, 0, cfg.stack_size);
  thread->cfg = cfg;
  thread->exec.call.func = func;
  thread->exec.call.args = args;
  thread->state = TS_STARTING;
  threadListAdd(__system.runnable.prev, thread);
  return 0;
}

int Run() {
  while (!threadListEmpty(&__system.runnable)) {
    Thread *volatile run = threadListPopFront(&__system.runnable);

    __system.current = run;
    __system.run_stack_pos = (void *)&run;

    switch (setjmp(__system.return_jump)) {
    case JC_SETJUMP:
      // The return jump was prepared, break to the second switch with
      // the current runnable thread.
      break;
    case JC_SUSPEND:
      // Running thread yielded.
      threadListAdd(__system.runnable.prev, run);
      continue;
    case JC_BLOCKED:
      // Running thread became blocked by another.
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
      // Run a new thread.
      run->state = TS_RUNNING;
      run->exec.call.func(TID(run), run->exec.call.args);
      run->state = TS_FINISHED;
      break;
    case TS_RUNNING:
      // Re-enter the yield call.
      longjmp(run->exec.run_jump, JC_RESUME);
      break;
      // Function call returned
    case TS_FINISHED:
      // Error cases exit here.
      break;
    }
  }
  return 0;
}

void channelInit(Channel *ch, int32_t flags) {
  ch->flags = flags;
  threadListInit(&ch->readers);
  threadListInit(&ch->writers);
}

int32_t channelAvailable(Channel *ch) {
  return ch->head - ch->tail;
}

int channelRead(Channel *ch, int32_t ch_size, void *data, size_t data_size) {
  int32_t avail;
again:
  avail = channelAvailable(ch);
  if (data_size > avail) {
    threadListAdd(ch->readers.prev, __system.current);
    yieldInternal(JC_BLOCKED);
    goto again;
  }
  int32_t take = data_size;
  int32_t tidx = ch->tail % ch_size;
  if (tidx + take > ch_size) {
    take = ch_size - tidx;
  }
  memcpy((uint8_t *)data, ch->buffer + tidx, take);
  memcpy(((uint8_t *)data) + take, ch->buffer, data_size - take);

  ch->tail += data_size;

  if (threadListEmpty(&ch->writers)) {
    return 0;
  }
  Thread *next = threadListPopFront(&ch->writers);
  threadListAdd(__system.runnable.prev, next);
  return 0;
}

void channelWrite(Channel *ch, int32_t ch_size, void *data, size_t data_size) {
  if (data_size > ch_size) {
    journal2u("write too large: %u > %u", data_size, ch_size);
    longjmp(__system.return_jump, JC_INTERNAL);
  }
  int32_t limit;
again:
  limit = ch->head + data_size;
  if (limit > ch->tail + ch_size) {

    if ((ch->flags & CF_NONBLOCKING) == 0) {
      // No room to write.
      threadListAdd(ch->writers.prev, __system.current);
      yieldInternal(JC_BLOCKED);
      goto again;
    } else {
      // Non-blocking channel full => data loss.
      int32_t new_tail = limit - ch_size;
      ch->lost += new_tail - ch->tail;
      ch->tail = new_tail;
    }
  }

  int32_t hidx = ch->head % ch_size;
  int32_t take = data_size;
  if (hidx + take > ch_size) {
    take = ch_size - hidx;
  }

  memcpy(ch->buffer + hidx, (uint8_t *)data, take);
  memcpy(ch->buffer, ((uint8_t *)data) + take, data_size - take);

  ch->head += data_size;

  if (threadListEmpty(&ch->readers)) {
    return;
  }
  Thread *next = threadListPopFront(&ch->readers);
  threadListAdd(__system.runnable.prev, next);
}

int journalRead(LogEntry *entry) {
  return channelRead(&__system.log.ch, sizeof(__system.log.space), entry, sizeof(*entry));
}

void journal2u(const char *msg, int32_t arg1, int32_t arg2) {
  LogEntry ent;
  ent.tid = TID(__system.current);
  ent.msg = msg;
  ent.arg1 = arg1;
  ent.arg2 = arg2;
  channelWrite(&__system.log.ch, sizeof(__system.log.space), &ent, sizeof(ent));
}

void Yield() {
  yieldInternal(JC_SUSPEND);
}

void yieldInternal(JumpCode jc) {
  void *volatile unused;
  void   *yield_stack = (void *)&unused;
  int32_t size = (int32_t)((size_t)__system.run_stack_pos - (size_t)yield_stack);

  // Check for thread-stack overflow.
  if (size > __system.current->cfg.stack_size) {
    journal2u("stack overflow: %u exceeds %u", size, __system.current->cfg.stack_size);
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

  // size and yield_stack are not volatile, recompute:
  yield_stack = (void *)&unused;
  size = (size_t)__system.run_stack_pos - (size_t)yield_stack;

  memcpy(yield_stack, __system.current->cfg.stack, size);
}

#ifdef __cplusplus
}
#endif
