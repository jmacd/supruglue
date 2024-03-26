#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/args/args.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/gpio/gpio.h"
#include "lib/initproc/initproc.h"
#include "lib/intc/intc.h"
#include "lib/intc/service.h"
#include "lib/log/daemon/daemon.h"
#include "lib/log/journal/journal.h"
#include "lib/pinmap/pinmap.h"
#include "lib/resource/table.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock.h"
#include "lib/time/process.h"
#include "lib/ui1203/ui1203.h"

#define PERIOD (2000000000U / 5)

UI1203_Reader reader;
UI1203_Writer writer;

int main(void) {
  Args args;

  Init(NewSystemConfig());
  InterruptServiceInit();
  ClockInit();
  RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);
  GPIO_Init();
  SyslogInit();
  ProcessInit();

  UI1203_Init_Reader(&reader, GPIO_PIN(P9_23));
  UI1203_Init_Writer(&writer, GPIO_PIN(P9_25));

  ControllerEnable();

  return Run();
}
