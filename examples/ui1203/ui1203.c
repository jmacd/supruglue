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

#define PERIOD (2000000000U / 5)

void runBlue(ThreadID tid, Args args) {
  gpio_pin pinA = GPIO_PIN(P9_23);
  gpio_pin pinB = GPIO_PIN(P9_25);

  PRULOG_1u32(INFO, "starting reader %uns", PERIOD / 2);

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    uint32_t value = GPIO_GetPin(pin);

    uint32_t val1 = 1;
    uint32_t val2 = 2;

    PRULOG_2u32(INFO_NOYIELD, "esr1 %u etflg %u", val1, val2);

    SleepUntil(&clock, PERIOD / 2);
  }
}

SUPRUGLUE_DEFINE_THREAD(blue, 256);

int main(void) {
  Args args;

  Init(NewSystemConfig());
  InterruptServiceInit();
  ClockInit();
  RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);
  GPIO_Init();
  SyslogInit();
  ProcessInit();

  args.ptr = "0";

  Create(&blue.thread, runBlue, args, "blue", sizeof(blue.space));

  ControllerEnable();

  return Run();
}
