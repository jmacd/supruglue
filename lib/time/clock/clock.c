// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/time/clock/clock.h"
#include "lib/coroutine/coroutine.h"
#include "lib/thread/thread.h"

#include <stdio.h>

SUPRUGLUE_DEFINE_THREAD(clockproc, 256);

// Note: I tested an implementation based on a heap, was about 500
// bytes larger.

ThreadList __asleep;

void Sleep(uint32_t d) {
  Thread *self = __system_current;

  ReadClock(&self->when);
  TimeAdd(&self->when, d);

  printf("go to sleep %qd\n", self->when.NANOS);
  ThreadListPushBack(&__asleep, self);

  YieldBlocked();
  printf("wakeup\n");
}

void clockProcess(ThreadID thid, Args args) {
  while (1) {
    Timestamp clk;
    ReadClock(&clk);

    ThreadList *p = __asleep.next;
    int         c = 0;
    while (p != &__asleep) {
      p = p->next;
      c++;
    }

    printf("checksleep run: %d %p\n", c, &__asleep);

    p = __asleep.next;
    while (p != &__asleep) {
      Thread *th = ThreadListEntry(p);

      printf("with %qd >= %qd (%d)\n", clk.NANOS, th->when.NANOS, clk.NANOS > th->when.NANOS);
      if (clk.NANOS >= th->when.NANOS) {
        ThreadListDelete(p->next, p->prev);
        ThreadListPushBack(&__system_runnable, th);
      }
      printf("n/p %p %p\n", p->next, p->prev);
      printf("th %p\n", &th->list);
      p = p->next;
    }

    printf("checksleep yield\n");
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
