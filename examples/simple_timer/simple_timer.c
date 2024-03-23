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

#define SECOND 200000000

// TODO: Configure these from the environment.
#define DUTY 1
#define CYCLE 24
#define PERIOD 3600

#define ON_FOR ((DUTY * PERIOD) / CYCLE)
#define OFF_FOR (((CYCLE - DUTY) * PERIOD) / CYCLE)

void toggle_timer(ThreadID tid, Args args) {
  gpio_pin pinO = GPIO_PIN(P8_11);
  gpio_pin pinC = GPIO_PIN(P8_12);

  Timestamp clock;
  ReadClock(&clock);

  uint64_t oncycles = SECOND;
  oncycles *= ON_FOR;

  uint64_t offcycles = SECOND;
  offcycles *= OFF_FOR;

  while (1) {
    PRULOG_1u64(INFO, "simple timer on 8.11 off 8.12 %us", ON_FOR);

    GPIO_SetPin(pinO, 1);
    GPIO_SetPin(pinC, 0);

    SleepUntil64(&clock, oncycles);

    PRULOG_1u64(INFO, "simple timer off 8.11 on 8.12 %us", OFF_FOR);

    GPIO_SetPin(pinO, 0);
    GPIO_SetPin(pinC, 1);

    SleepUntil64(&clock, offcycles);
  }
}

SUPRUGLUE_DEFINE_THREAD(timer, 256);

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

  Create(&timer.thread, toggle_timer, args, "timer", sizeof(timer.space));

  ControllerEnable();

  return Run();
}
