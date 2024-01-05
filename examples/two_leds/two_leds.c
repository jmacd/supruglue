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

// Note: the argument is cycles / 5 because 5 ns cycle
#define BLUE_PERIOD (2000000000U / 5)
#define YELLOW_PERIOD (1000000000U / 5)

void toggle_blue(ThreadID tid, Args args) {
  gpio_pin pin = GPIO_PIN(P9_23);
  PRULOG_1u32(INFO, "starting blue half-cycle %uns", BLUE_PERIOD);

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    PRULOG_0(INFO, "on");

    GPIO_SetPin(pin, 1);
    SleepUntil(&clock, BLUE_PERIOD);

    PRULOG_0(INFO, "off");

    GPIO_SetPin(pin, 0);
    SleepUntil(&clock, BLUE_PERIOD);
  }
}

void toggle_yellow(ThreadID tid, Args args) {
  gpio_pin pin = GPIO_PIN(P9_25);
  PRULOG_1u32(INFO, "starting yellow half-cycle %uns", YELLOW_PERIOD);

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    PRULOG_0(INFO, "on");

    GPIO_SetPin(pin, 1);
    SleepUntil(&clock, YELLOW_PERIOD);

    PRULOG_0(INFO, "off");

    GPIO_SetPin(pin, 0);
    SleepUntil(&clock, YELLOW_PERIOD);
  }
}

SUPRUGLUE_DEFINE_THREAD(blue, 256);
SUPRUGLUE_DEFINE_THREAD(yellow, 256);

int main(void) {
  Args args;

  Init(NewSystemConfig());
  InterruptServiceInit();
  ClockInit();
  RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);
  SyslogInit();
  ProcessInit();

  args.ptr = "0";

  Create(&blue.thread, toggle_blue, args, "blue", sizeof(blue.space));
  Create(&yellow.thread, toggle_yellow, args, "yellow", sizeof(yellow.space));

  return Run();
}
