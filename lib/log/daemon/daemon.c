// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "daemon.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock.h"

void SyslogProcess(ThreadID thid, Args args) {
  for (;;) {
    TypedEntry tentry;
    tentry.typeid = 1;

    JournalRead(&__system.journal, &tentry.entry, JR_BLOCKING);

    int err;
    while ((err = ClientSend(&__transport, &tentry, sizeof(tentry))) != 0) {
      // TODO: Flash user LEDs?
      Sleep(1000000);
    }

    Yield();
  }
}
