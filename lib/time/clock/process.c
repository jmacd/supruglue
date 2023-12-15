// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "process.h"
#include "clock.h"
#include "lib/coroutine/coroutine.h"
#include "lib/intc/intc.h"
#include "lib/soc/sysevts.h"

SUPRUGLUE_DEFINE_THREAD(clockproc, 256);

void clockProcess(ThreadID thid, Args args) {
  while (1) {
    BlockOnSystemEvent(&__controller, SYSEVT_PR1_IEP_TIM_CAP_CMP_PEND);

    // TODO why this won't quite work is we need to clear the timer
    // compare status (and probably delay 2 cycles, see pru_cape
    // examples) before clearing the interrupt condition

    Timestamp clk;
    ReadClock(&clk); // This has clears the timer compare status

    ClearSystemEvent(SYSEVT_PR1_IEP_TIM_CAP_CMP_PEND);

    ThreadList *p = __asleep.next;
    while (p != &__asleep) {
      Thread *th = ThreadListEntry(p);

      int runnable = clk >= th->when;
      if (runnable) {
        ThreadListRemove(th);
      }
      p = p->next;
      if (runnable) {
        ThreadListPushBack(&__system_runnable, th);
      }
    }
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
