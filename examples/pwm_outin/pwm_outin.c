#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// @@@ TODO: here for diagnostics, remove!
#include "external/ti-pru-support/include/am335x/pru_intc.h"
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
  uint32_t clk = PWMSS1.EPWM_TBCNT;
  PRULOG_2u32(INFO, "interrupt EPWM1 output A %u %u", value, clk);
  PWM_ClearInterrupt();
}

void runBlue(ThreadID tid, Args args) {
  gpio_pin pin = GPIO_PIN(P9_12);

  PRULOG_1u32(INFO, "starting reader %uns", PERIOD / 2);
  // PWM_ClearInterrupt();

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    uint32_t value = GPIO_GetPin(pin);

    // uint32_t val1 = PWMSS1.EPWM_TBCNT;
    // uint32_t val2 = PWMSS1.EPWM_ETPS;
    // uint32_t val2 = PWMSS1.EPWM_ETFLG;
    // uint32_t val2 = PWMSS1.EPWM_ETSEL;
    // uint32_t val2 = PWMSS1.EPWM_CMPB;
    // uint32_t val2 = EDMA_BASE[SHADOW1(EDMAREG_SERH)];
    uint32_t val1;
    uint32_t val2;

    val1 = PWMSS1.EPWM_TBCNT;
    val2 = PWMSS1.EPWM_ETFLG;

    PRULOG_2u32(INFO_NOYIELD, "tbcntx(b) %u etflg %u", val1, val2);

    // PWM_ClearInterrupt();

    val1 = PWMSS1.EPWM_TBCNT;
    val2 = PWMSS1.EPWM_ETFLG;

    PRULOG_2u32(INFO_NOYIELD, "tbcnt(a) %u etflg %u", val1, val2);

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

  InterruptHandlerInit(SYSEVT_TPCC_INT_PEND_PO1, pwmHandler);

  PWM_ClearInterrupt();

  int32_t flag = PWMSS1.EPWM_ETFLG;
  int32_t clk = PWMSS1.EPWM_TBCNT;

  PRULOG_2u32(INFO_NOYIELD, "about to enable %u %u", flag, clk);

  PWM_Enable();

  flag = PWMSS1.EPWM_ETFLG;
  clk = PWMSS1.EPWM_TBCNT;
  PRULOG_2u32(INFO_NOYIELD, "now enabled %u %u", flag, clk);

  // @@@ TODO/Note: this call has to be added in other tests, following PWM_Enable, examples...
  ControllerEnable();

  return Run();
}
