#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "external/ti-pru-support/include/am335x/sys_pwmss.h"

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
#include "lib/pwm/pwm.h"
#include "lib/resource/table.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/soc/sysevts.h"
#include "lib/time/clock.h"
#include "lib/time/process.h"

#define PERIOD (2000000000U / 5)

void pwmHandler(void) {
  gpio_pin pin = GPIO_PIN(P9_12);
  uint32_t value = GPIO_GetPin(pin);
  PRULOG_1u32(INFO, "interrupt EPWM1 output A", value);
  PWM_ClearInterrupt();
}

void runBlue(ThreadID tid, Args args) {
  gpio_pin pin = GPIO_PIN(P9_12);

  PRULOG_1u32(INFO, "starting reader %uns", PERIOD / 2);
  PWM_ClearInterrupt();

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    uint32_t value = GPIO_GetPin(pin);

    uint32_t tbcnt = PWMSS1.EPWM_TBCNT;
    //  uint32_t probe = PWMSS1.EPWM_ETPS;
    // uint32_t probe = PWMSS1.EPWM_ETFLG;
    // uint32_t probe = EDMA_BASE[SHADOW1(EDMAREG_SERH)];
    uint32_t probe = PWMSS1.EPWM_ETSEL;
    // uint32_t probe = PWMSS1.EPWM_CMPB;

    PRULOG_2u32(INFO_NOYIELD, "read %u probe %u", tbcnt, probe);

    PWMSS1.EPWM_ETCLR = 1;

    SleepUntil(&clock, PERIOD / 2);
  }
}

SUPRUGLUE_DEFINE_THREAD(blue, 256);

int main(void) {
  Args args;

  Init(NewSystemConfig());
  PWM_Init();
  InterruptServiceInit();
  ClockInit();
  RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);
  GPIO_Init();
  SyslogInit();
  ProcessInit();

  args.ptr = "0";

  Create(&blue.thread, runBlue, args, "blue", sizeof(blue.space));

  InterruptHandlerInit(SYSEVT_EPWM1_INTR_PEND, pwmHandler);

  PWM_Enable();

  // @@@ TODO need to add this call in other tests, examples...
  ControllerEnable();

  return Run();
}
