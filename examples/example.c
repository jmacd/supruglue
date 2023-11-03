#include "external/ti-pru-support/include/pru_virtio_ids.h"
#include "lib/args/args.h"
#include "lib/cpx/cpx.h"
#include "lib/log/daemon/daemon.h"
#include "lib/rpmsg/am335x/rpmsg_am335x.h"
#include <stddef.h>

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

void test_write_func(ThreadID tid, Args args) {
  int32_t cnt = Atoi(args.ptr);
  int32_t i;
  for (i = 0; i < cnt; i++) {
    PRULOG_2U(INFO, "write %u", i, 0); // Logs always yield
  }
}

void main() {
  int err =
      RpmsgInit(&__transport, &resourceTable.rpmsg_vdev, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1);

  Init(NewSystemConfig());

  Thread  writer;
  Thread  syslog;
  uint8_t stack0[500];
  uint8_t stack1[500];
  Args    args1, args2;
  args1.ptr = "1";
  args2.ptr = "";

  err = Create(&writer, test_write_func, args1, NewThreadConfig("writer", stack0, sizeof(stack0)));
  err = Create(&syslog, SyslogProcess, args2, NewThreadConfig("syslog", stack1, sizeof(stack1)));

  err = Run();
}
