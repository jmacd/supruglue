// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

// TODO: This is a placeholder.  Presently there is nothing
// configurable here--all the examples use the same resource table.

#include "external/ti-pru-support/include/rsc_types.h"

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

extern struct my_resource_table resourceTable;
