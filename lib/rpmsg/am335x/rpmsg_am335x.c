// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "rpmsg_am335x.h"
#include "external/ti-pru-support/include/pru_rpmsg.h"
#include "lib/rpmsg/rpmsg_iface.h"

// These two system events are wired to the PRU automatically by the
// pru_rpmsg driver.
//
// The am335x hardware does not require 16/17 be used for PRU0 and
// 18/19 be used for PRU1, but it is configured this way in the the
// default device tree, e.g. for pru0 see am33xx.dtsi in bb.org's
// fork.
//
// pru0: pru@4a334000 {
//     ...
//     interrupts = <16>, <17>;
//     interrupt-names = "vring", "kick";
// }
//
// pru1: pru@4a338000 {
//     ...
//     interrupts = <18>, <19>;
//     interrupt-names = "vring", "kick";
// }
//
// In the TRM these interrupts are labeled pr1_pru_mst_intr[0,1,2,3]_intr_req
// in section 4.4.2.2 PRU-ICSS System Events, table 4.22.

// sysevt 16 == pr1_pru_mst_intr[0]_intr_req
#define SYSEVT_PRU_TO_ARM 16

// sysevt 17 == pr1_pru_mst_intr[1]_intr_req
#define SYSEVT_ARM_TO_PRU 17

struct _ClientTransport {
  struct pru_rpmsg_transport channel;
};

int RpmsgInit(ClientTransport *transport, struct fw_rsc_vdev_vring *vring0, struct fw_rsc_vdev_vring *vring1) {
  // Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
  // at linux/drivers/rpmsg/rpmsg_pru.c.
  //
  // TODO: this is also configured in the user-space program, via the device file.
  char      channel_name[32] = "rpmsg-pru";
  const int channel_port = 30;

  // Initialize two vrings using system events on dedicated channels.
  pru_rpmsg_init(&transport->channel, vring0, vring1, SYSEVT_PRU_TO_ARM, SYSEVT_ARM_TO_PRU);

  // Create the RPMsg channel between the PRU and the ARM.
  while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport->channel, channel_name, channel_port) != PRU_RPMSG_SUCCESS) {
  }

  return 0;
}

int ClientSend(ClientTransport *transport, const void *data, uint16_t len) {
  return 0;
}

int ClientRecv(ClientTransport *transport, void *data, uint16_t *len) {
  return 0;
}
