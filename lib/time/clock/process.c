// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "process.h"
#include "clock.h"
#include "lib/coroutine/coroutine.h"

SUPRUGLUE_DEFINE_THREAD(clockproc, 256);

void clockProcess(ThreadID thid, Args args) {
  while (1) {
    Timestamp clk;
    ReadClock(&clk);

    ThreadList *p = __asleep.next;
    while (p != &__asleep) {
      Thread *th = ThreadListEntry(p);

      int runnable = clk.NANOS >= th->when.NANOS;
      if (runnable) {
        ThreadListRemove(th);
      }
      p = p->next;
      if (runnable) {
        ThreadListPushBack(&__system_runnable, th);
      }
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
