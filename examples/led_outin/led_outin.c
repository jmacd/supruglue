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
#define PERIOD (2000000000U / 5)

void toggle_blue(ThreadID tid, Args args) {
  gpio_pin pin = GPIO_PIN(P9_23);
  PRULOG_1u32(INFO, "starting blue half-cycle %uns", PERIOD);

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    PRULOG_0(INFO, "on");

    GPIO_SetPin(pin, 1);
    SleepUntil(&clock, PERIOD);

    PRULOG_0(INFO, "off");

    GPIO_SetPin(pin, 0);
    SleepUntil(&clock, PERIOD);
  }
}

void read_yellow(ThreadID tid, Args args) {
  gpio_pin pin = GPIO_PIN(P9_25);

  // GPIO_EnableInput(pin);

  PRULOG_1u32(INFO, "starting yellow reader %uns", PERIOD / 2);

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    uint32_t value = GPIO_GetPin(pin);

    PRULOG_1u32(INFO, "yellow is %u", value);

    SleepUntil(&clock, PERIOD / 2);
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
  Create(&yellow.thread, read_yellow, args, "yellow", sizeof(yellow.space));

  return Run();
}
