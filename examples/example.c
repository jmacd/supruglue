#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "external/ti-pru-support/include/am335x/pru_cfg.h"
#include "external/ti-pru-support/include/pru_virtio_ids.h"
#include "lib/args/args.h"
#include "lib/coroutine/coroutine.h"
#include "lib/debug/debug.h"
#include "lib/gpio/gpio.h"
#include "lib/initproc/initproc.h"
#include "lib/intc/service.h"
#include "lib/log/daemon/daemon.h"
#include "lib/pinmap/pinmap.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/clock.h"
#include "lib/time/process.h"

#define NUM_RESOURCES 1

// my_resource_table describes the custom hardware settings used by
// this program.
struct my_resource_table {
  struct resource_table base;

  uint32_t offset[NUM_RESOURCES]; // Should match 'num' in actual definition

  struct fw_rsc_vdev       rpmsg_vdev;   // Resource 0
  struct fw_rsc_vdev_vring rpmsg_vring0; // (cont)
  struct fw_rsc_vdev_vring rpmsg_vring1; // (cont)
};

// Set in resourceTable.rpmsg_vdev.status when the kernel is ready.
#define VIRTIO_CONFIG_S_DRIVER_OK ((uint32_t)1 << 2)

// Sizes of the virtqueues (expressed in number of buffers supported,
// and must be power of 2)
#define PRU_RPMSG_VQ0_SIZE 16
#define PRU_RPMSG_VQ1_SIZE 16

// The feature bitmap for virtio rpmsg
#define VIRTIO_RPMSG_F_NS 0 // name service notifications

// This firmware supports name service notifications as one of its features.
#define RPMSG_PRU_C0_FEATURES (1 << VIRTIO_RPMSG_F_NS)

#pragma DATA_SECTION(resourceTable, ".resource_table")
#pragma RETAIN(resourceTable)
struct my_resource_table resourceTable = {
    // resource_table base
    {
        1,             // Resource table version: only version 1 is supported
        NUM_RESOURCES, // Number of entries in the table (equals length of offset field).
        0, 0,          // Reserved zero fields
    },
    // Entry offsets
    {
        offsetof(struct my_resource_table, rpmsg_vdev),
    },
    // RPMsg virtual device
    {
        (uint32_t)TYPE_VDEV,             // type
        (uint32_t)VIRTIO_ID_RPMSG,       // id
        (uint32_t)0,                     // notifyid
        (uint32_t)RPMSG_PRU_C0_FEATURES, // dfeatures
        (uint32_t)0,                     // gfeatures
        (uint32_t)0,                     // config_len
        (uint8_t)0,                      // status
        (uint8_t)2,                      // num_of_vrings, only two is supported
        {(uint8_t)0, (uint8_t)0},        // reserved
    },
    // The two vring structs must be packed after the vdev entry.
    {
        FW_RSC_ADDR_ANY,    // da, will be populated by host, can't pass it in
        16,                 // align (bytes),
        PRU_RPMSG_VQ0_SIZE, // num of descriptors
        0,                  // notifyid, will be populated, can't pass right now
        0                   // reserved
    },
    {
        FW_RSC_ADDR_ANY,    // da, will be populated by host, can't pass it in
        16,                 // align (bytes),
        PRU_RPMSG_VQ1_SIZE, // num of descriptors
        0,                  // notifyid, will be populated, can't pass right now
        0                   // reserved
    },
};

// Note: the argument is cycles / 5 because 5 ns cycle
#define BLUE_PERIOD (2000000000U / 5)
#define YELLOW_PERIOD (1000000000U / 5)

void toggle_blue(ThreadID tid, Args args) {
  gpio_pin pin = GPIO_PIN(P9_23);
  PRULOG_1u32(INFO, "starting blue half-cycle %uns", BLUE_PERIOD);

  Timestamp clock;
  ReadClock(&clock);
  while (1) {
    PRULOG_0(INFO, "blue on");

    GPIO_SetPin(pin, 1);
    SleepUntil(&clock, BLUE_PERIOD);

    PRULOG_0(INFO, "blue off");

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
    PRULOG_0(INFO, "yellow on");

    GPIO_SetPin(pin, 1);
    SleepUntil(&clock, YELLOW_PERIOD);

    PRULOG_0(INFO, "yellow off");

    GPIO_SetPin(pin, 0);
    SleepUntil(&clock, YELLOW_PERIOD);
  }
}

SUPRUGLUE_DEFINE_THREAD(syslog, 256);
SUPRUGLUE_DEFINE_THREAD(init, 256);
SUPRUGLUE_DEFINE_THREAD(blue, 256);
SUPRUGLUE_DEFINE_THREAD(yellow, 256);

int main(void) {

  Args args1;
  Args args2;
  int  err = 0;

  Init(NewSystemConfig());

  InterruptServiceInit();
  ClockInit();

  err = RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);
  if (err != 0) {
    // @@@ this has failed b/c wrong event number... what would we do?
    // (q: why compile-in such checks? is there a way to panic?)
  }

  args1.ptr = "1";
  args2.ptr = "0";

  err = Create(&init.thread, InitProcess, args1, "init", sizeof(init.space));
  err = Create(&syslog.thread, SyslogProcess, args2, "syslog", sizeof(syslog.space));

  err = Create(&blue.thread, toggle_blue, args2, "blue", sizeof(blue.space));
  err = Create(&yellow.thread, toggle_yellow, args2, "yellow", sizeof(yellow.space));

  err = Run();
  return err;
}
