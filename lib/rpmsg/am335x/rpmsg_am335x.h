// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_AM335X_RPMSG_AM335X_H
#define LIB_RPMSG_AM335X_RPMSG_AM335X_H

#include "external/ti-pru-support/include/rsc_types.h"
#include "lib/rpmsg/rpmsg_iface.h"

#define PRU_RPMSG_NO_PEER_ADDR (-5)

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

#endif // LIB_RPMSG_AM335X_RPMSG_AM335X_H
