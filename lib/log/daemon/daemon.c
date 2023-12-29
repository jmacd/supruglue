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

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    Entry entry;

    JournalRead(&__system.journal, &entry, JR_BLOCKING);

    int err;
    while ((err = ClientSend(&__transport, &entry, sizeof(entry))) != 0) {
      // TODO: Flash user LEDs?
      Sleep(1000000);
    }

    Yield();
  }
}
