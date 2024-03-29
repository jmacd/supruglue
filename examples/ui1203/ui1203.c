#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/args/args.h"
#include "lib/cap/cap.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/gpio/gpio.h"
#include "lib/initproc/initproc.h"
#include "lib/intc/intc.h"
#include "lib/intc/service.h"
#include "lib/log/daemon/daemon.h"
#include "lib/log/journal/journal.h"
#include "lib/pinmap/pinmap.h"
#include "lib/pwm/pwm.h"
#include "lib/resource/table.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock.h"
#include "lib/time/process.h"
#include "lib/ui1203/ui1203.h"

#define PERIOD (2000000000U / 5)

UI1203_Reader reader;
UI1203_Writer writer;

void runBlue(ThreadID tid, Args args) {

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    PRULOG_0(INFO_NOYIELD, "blue");

    SleepUntil32(&clock, PERIOD);
  }
}

SUPRUGLUE_DEFINE_THREAD(blue, 256);

int main(void) {
  Init(NewSystemConfig());

  PWM_Init(5000, 10000, 2500);
  CAP_Init();

  InterruptServiceInit();
  ClockInit();
  RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);
  GPIO_Init();
  SyslogInit();
  ProcessInit();

  UI1203_Init_Reader(&reader, GPIO_PIN(P9_25));
  UI1203_Init_Writer(&writer, GPIO_PIN(P9_23));

  Args args;
  args.ptr = "0"; // @@@
  Create(&blue.thread, runBlue, args, "blue", sizeof(blue.space));

  PWM_ClearInterrupt();
  PWM_Enable();

  CAP_ClearInterrupt();
  CAP_Enable();

  ControllerEnable();

  return Run();
}
