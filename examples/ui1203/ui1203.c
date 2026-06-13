// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/args/args.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/gpio/gpio.h"
#include "lib/initproc/initproc.h"
#include "lib/intc/service.h"
#include "lib/log/daemon/daemon.h"
#include "lib/pinmap/pinmap.h"
#include "lib/resource/table.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/clock.h"
#include "lib/time/process.h"
#include "lib/ui1203/ui1203.h"

// Provisional pin assignment (pending final selection): clock/power drives the
// meter's RED line, data reads the open-collector GREEN line (pulled up).
#define UI1203_CLOCK_PIN GPIO_PIN(P9_25)
#define UI1203_DATA_PIN GPIO_PIN(P9_23)

UI1203_Reader reader;

int main(void) {
  Init(NewSystemConfig());

  InterruptServiceInit();
  ClockInit();
  RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);
  GPIO_Init();
  SyslogInit();
  ProcessInit();

  UI1203_Init_Reader(&reader, UI1203_CLOCK_PIN, UI1203_DATA_PIN);

  ControllerEnable();

  return Run();
}
