// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_AM335X_RPMSG_AM335X_H
#define LIB_RPMSG_AM335X_RPMSG_AM335X_H

#include "external/ti-pru-support/include/pru_rpmsg.h"
#include "external/ti-pru-support/include/rsc_types.h"
#include "lib/rpmsg/rpmsg_iface.h"

#define PRU_RPMSG_NO_PEER_ADDR (-5)

#ifdef __cplusplus
extern "C" {
#endif

struct _ClientTransport {
  struct pru_rpmsg_transport channel;

  // This event needs to be cleared when messages are received.
  int8_t sysevt_arm_to_pru;

  // Note: maybe unused
  int16_t sysevt_pru_to_arm;

  // The peer address is initially 0, which case with no messages ever
  // received, we can't really send.
  uint16_t rpmsg_peer_src_addr;

  // Equals the message source address.
  int16_t channel_port;
};

// rpmsg is initialized through a static resource table, which is identified
// via a linker command file (e.g., AM335x_PRU_intc_rscTbl.cmd)
//
// In that object, there will be two struct fw_rsc_vdev_vring, which must
// be passed here.
//
// TODO: How does this function get the Args.  How does anyone get args?
// Maybe it doesn't? This can be a linker-command configuration since we
// will use rpmsg to get the args etc.
int RpmsgInit(ClientTransport *transport, struct fw_rsc_vdev *vdev, struct fw_rsc_vdev_vring *vring0,
              struct fw_rsc_vdev_vring *vring1);

#ifdef __cplusplus
}
#endif

#endif // LIB_RPMSG_AM335X_RPMSG_AM335X_H
