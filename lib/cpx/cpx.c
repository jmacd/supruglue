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

void journal2u(const char *msg, size_t arg1, size_t arg2);

System __system;

SystemConfig NewSystemConfig() {
  return (SystemConfig){};
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
      break;
    case JC_CONTINUING:
      threadListAdd(__system.runnable.prev, run);
      continue;
    case JC_OVERFLOW:
    case JC_INTERNAL:
      run->state = TS_FINISHED; // TODO: or error
    }
    switch (run->state) {
    case TS_STARTING:
      run->state = TS_RUNNING;
      run->exec.call.func(TID(run), run->exec.call.args);
      run->state = TS_FINISHED;
      break;
    case TS_RUNNING:
      longjmp(run->exec.run_jump, JC_CONTINUING);
      break;
    case TS_FINISHED:
      break;
    }
  }
  return 0;
}

int32_t channelAvailable(Channel *ch) {
  return ch->head - ch->tail;
}

void channelRead(Channel *ch, int32_t ch_size, void *data, size_t data_size) {
  int32_t avail = channelAvailable(ch);
  if (data_size > avail) {
    // @@@
    fprintf(stderr, "no data in channel %u!\n", avail);
    assert(0);
    return;
  }
  int32_t take = data_size;
  int32_t tidx = ch->tail % ch_size;
  if (tidx + take > ch_size) {
    take = ch_size - tidx;
  }
  memcpy((uint8_t *)data, ch->buffer + tidx, take);
  memcpy(((uint8_t *)data) + take, ch->buffer, data_size - take);

  ch->tail += data_size;
}

void channelWrite(Channel *ch, int32_t ch_size, void *data, size_t data_size) {
  if (data_size > ch_size) {
    journal2u("write too large: %u > %u", data_size, ch_size);
    longjmp(__system.return_jump, JC_INTERNAL);
  }

  int32_t limit = ch->head + data_size;
  if (limit > ch->tail + ch_size) {
    // Note: data loss
    fprintf(stderr, "DATA LOSS\n");
    assert(0);
    ch->tail = limit - ch_size;
  }

  int32_t hidx = ch->head % ch_size;
  int32_t take = data_size;
  if (hidx + take > ch_size) {
    take = ch_size - hidx;
  }

  memcpy(ch->buffer + hidx, (uint8_t *)data, take);
  memcpy(ch->buffer, ((uint8_t *)data) + take, data_size - take);

  ch->head += data_size;
}

void journal2u(const char *msg, size_t arg1, size_t arg2) {
  LogEntry ent;
  ent.tid = TID(__system.current);
  ent.msg = msg;
  ent.arg1 = arg1;
  ent.arg2 = arg2;

  // fprintf(stderr, "journal: ");
  // fprintf(stderr, msg, arg1, arg2);
  // fprintf(stderr, "\n");

  channelWrite(&__system.log.base, sizeof(__system.log.space), &ent, sizeof(ent));
}

void journalBogus(void) {
  LogEntry toomany[LOG_CHANNEL_ENTRIES + 1];
  channelWrite(&__system.log.base, sizeof(__system.log.space), &toomany, sizeof(toomany));
}

void Yield() {
  void *volatile unused;
  void   *yield_stack = (void *)&unused;
  int32_t size = (int32_t)((size_t)__system.run_stack_pos - (size_t)yield_stack);

  // Check for thread-stack overflow.
  if (size > __system.current->cfg.stack_size) {
    journal2u("stack overflow: %u exceeds %u", size, __system.current->cfg.stack_size);
    longjmp(__system.return_jump, JC_OVERFLOW);
  }
  memcpy(__system.current->cfg.stack, yield_stack, size);

  if (setjmp(__system.current->exec.run_jump) == JC_SETJUMP) {
    longjmp(__system.return_jump, JC_CONTINUING);
  }

  // size and yield_stack are not volatile, recompute:
  yield_stack = (void *)&unused;
  size = (size_t)__system.run_stack_pos - (size_t)yield_stack;

  memcpy(yield_stack, __system.current->cfg.stack, size);
}

#ifdef __cplusplus
}
#endif
