// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "daemon.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock.h"

void syslogProcess(ThreadID thid, Args args) {
  for (;;) {
    Entry entry;

    JournalRead(&__system.journal, &entry, JR_BLOCKING);

    while (ClientSend(&__transport, &entry, sizeof(entry)) != 0) {
      // TODO: Flash user LEDs?
      Sleep(TIME_SECOND / 2);
    }

    Yield();
  }
}

// The 512 byte stack is needed for host testing.  TODO: for on-PRU too?
SUPRUGLUE_DEFINE_THREAD(syslog, 512);

int SyslogInit(void) {
  Args args;
  args.ptr = "";
  return Create(&syslog.thread, syslogProcess, args, "syslog", sizeof(syslog.space));
}
