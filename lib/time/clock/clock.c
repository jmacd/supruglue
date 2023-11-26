// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/clock/clock.h"
#include "lib/coroutine/coroutine.h"
#include "lib/thread/thread.h"

SUPRUGLUE_DEFINE_THREAD(clockproc, 256);

// Note: I tested an implementation based on a heap, was about 500
// bytes larger.

ThreadList __asleep;

void Sleep(uint32_t d) {
  Thread *self = __system_current;

  ReadClock(&self->when);
  TimeAdd(&self->when, d);

  ThreadListPushBack(&__asleep, self);

  YieldBlocked();
}

void clockProcess(ThreadID thid, Args args) {
  while (1) {
    Timestamp clk;
    ReadClock(&clk);

    ThreadList *p = __asleep.next;
    while (p != &__asleep) {
      Thread *th = ThreadListEntry(p);

      if (clk.NANOS >= th->when.NANOS) {
        ThreadListDelete(p->prev, p->next);
        ThreadListPushBack(&__system_runnable, th);
      }
      p = p->next;
    }

    Yield();
  }
}

// ClockInit enables a handler to maintain the clock.
int ClockInit(void) {
  TimeInit();

  ThreadListInit(&__asleep);

  Args args;
  args.ptr = "";
  return Create(&clockproc.thread, clockProcess, args, "clock", sizeof(clockproc.space));
}

void TimeAdd(Timestamp *clock, uint32_t dur) {
  clock->NANOS += dur;
}
