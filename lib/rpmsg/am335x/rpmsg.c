// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "rpmsg.h"
#include <stdint.h>

#include "external/ti-pru-support/include/am335x/pru_intc.h"
#include "lib/debug/debug.h"
#include "lib/intc/intc.h"
#include "lib/rpmsg/rpmsg-defs.h"
#include "lib/soc/soc.h"
#include "lib/soc/sysevts.h"
#include <string.h>

ClientTransport __transport;

// The following is documented in pru-software-support-package
// and declared in linux-x.y.z/include/uapi/linux/virtio_config.h
// with the comment "Driver has used its parts of the config, and
// is happy".
#define VIRTIO_CONFIG_S_DRIVER_OK 4

// Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
// at linux/drivers/rpmsg/rpmsg_pru.c.
//
// Port numbers are not well documented.  Using 30 and 31 for pru0 and
// pru1 is common practice.
#define RPMSG_CHANNEL_NAME "rpmsg-pru"
#define RPMSG_CHANNEL_PORT_0 30
#define RPMSG_CHANNEL_PORT_1 31

void RpmsgKick(void) {
  // Note: this can wake > 1
  SemaUp(&__transport.kick_lock);
}

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
int RpmsgInit(ClientTransport *transport, struct fw_rsc_vdev *vdev, struct fw_rsc_vdev_vring *vring0,
              struct fw_rsc_vdev_vring *vring1) {
  uint8_t sysevt_pru_to_arm;
  uint8_t sysevt_arm_to_pru;
  memset(transport, 0, sizeof(*transport));

  LockInit(&transport->peer_lock);
  LockInit(&transport->kick_lock);

  // Ensure the virtio driver is ready.
  volatile uint8_t *status = &vdev->status;
  while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK)) {
  }

  // The system events and port are core-specific.
#if SUPRUGLUE_PRU_NUM == 0
  InterruptHandlerInit(SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ, &RpmsgKick);

  transport->channel_port = RPMSG_CHANNEL_PORT_0;
  sysevt_pru_to_arm = SYSEVT_PR1_PRU_MST_INTR0_INTR_REQ;
  sysevt_arm_to_pru = SYSEVT_PR1_PRU_MST_INTR1_INTR_REQ;
#elif SUPRUGLUE_PRU_NUM == 1
  InterruptHandlerInit(SYSEVT_PR1_PRU_MST_INTR3_INTR_REQ, &RpmsgKick);

  transport->channel_port = RPMSG_CHANNEL_PORT_1;
  sysevt_pru_to_arm = SYSEVT_PR1_PRU_MST_INTR2_INTR_REQ;
  sysevt_arm_to_pru = SYSEVT_PR1_PRU_MST_INTR3_INTR_REQ;
#else
#pragma "PRU number is not set"
#endif

  int ret;
  ret = pru_rpmsg_init(&transport->channel, vring0, vring1, sysevt_pru_to_arm, sysevt_arm_to_pru);
  if (ret != PRU_RPMSG_SUCCESS) {
    return ret;
  }

  // Create the RPMsg channel between the PRU and the ARM.
  while ((ret = pru_rpmsg_channel(RPMSG_NS_CREATE, &transport->channel, RPMSG_CHANNEL_NAME, transport->channel_port)) !=
         PRU_RPMSG_SUCCESS) {
  }

  return ret;
}

int ClientSend(ClientTransport *transport, const void *data, uint16_t len) {
  if (transport->peer_src_addr == 0) {
    // In case we have never received.
    SemaDown(&transport->peer_lock);
  }

  int err = pru_rpmsg_send(&transport->channel, transport->peer_dst_addr, transport->peer_src_addr, (void *)data, len);
  if (err != 0) {
    SemaDown(&transport->kick_lock);
  }
  return err;
}

int ClientRecv(ClientTransport *transport, void *data, uint16_t *len) {
  int was_unset = transport->peer_src_addr == 0;

  int err = pru_rpmsg_receive(&transport->channel, &transport->peer_src_addr, &transport->peer_dst_addr, data, len);
  if (err != 0) {
    SemaDown(&transport->kick_lock);
    return err;
  }
  if (was_unset != 0 && transport->peer_src_addr != 0) {
    SemaUp(&transport->peer_lock);
  }
  return 0;
}
