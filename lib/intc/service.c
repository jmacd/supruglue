// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "intc.h"
#include "service.h"

#include "lib/coroutine/coroutine.h"

SUPRUGLUE_DEFINE_THREAD(serviceproc, 256);

void serviceProcess(ThreadID thid, Args args) {
  for (;;) {
    if (ThreadListEmpty(&__system_runnable)) {
      SystemOnChipSuspend();
    }
    Yield();
    ServiceInterrupts();
  }
}

int InterruptServiceInit(void) {
  ControllerInit();

  Args args;
  args.ptr = "";
  return Create(&serviceproc.thread, serviceProcess, args, "intc", sizeof(serviceproc.space));
}
