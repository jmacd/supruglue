// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/rpmsg/rpmsg.h"
#include <pru_rpmsg.h>

typedef struct _ClientTransport ClientTransport;

int ClientSend(ClientTransport *transport, const void *data, uint16_t len);
int ClientRecv(ClientTransport *transport, void *data, uint16_t *len);

extern ClientTransport *__transport;

#if 0
struct pru_rpmsg_transport rpmsg_transport;
char                       rpmsg_payload[RPMSG_BUF_SIZE];
uint16_t                   rpmsg_src, rpmsg_dst, rpmsg_len;

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

struct fw_rsc_vdev       rpmsg_vdev;   // Resource 2
struct fw_rsc_vdev_vring rpmsg_vring0; // (cont)
struct fw_rsc_vdev_vring rpmsg_vring1; // (cont)

offsetof(struct my_resource_table, rpmsg_vdev),

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
}

// wait_for_virtio_ready waits for Linux drivers to be ready for RPMsg communication.
void wait_for_virtio_ready() {
  volatile uint8_t *status = &resourceTable.rpmsg_vdev.status;
  while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK)) {
    // Wait
  }
}

// setup_transport opens the RPMsg channel to the ARM host.
void setup_transport() {
  // Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
  // at linux/drivers/rpmsg/rpmsg_pru.c
  char      channel_name[32] = "rpmsg-pru";
  const int channel_port = 30;

  // Initialize two vrings using system events on dedicated channels.
  pru_rpmsg_init(&rpmsg_transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, SYSEVT_PRU_TO_ARM,
                 SYSEVT_ARM_TO_PRU);

  // Create the RPMsg channel between the PRU and the ARM.
  while (pru_rpmsg_channel(RPMSG_NS_CREATE, &rpmsg_transport, channel_name, channel_port) != PRU_RPMSG_SUCCESS) {
  }
}

// send_to_arm sends the carveout addresses to the ARM.
void send_to_arm() {
  if (pru_rpmsg_receive(&rpmsg_transport, &rpmsg_src, &rpmsg_dst, rpmsg_payload, &rpmsg_len) != PRU_RPMSG_SUCCESS) {
    return;
  }
  memcpy(rpmsg_payload, &resourceTable.controls.pa, 4);
  while (pru_rpmsg_send(&rpmsg_transport, rpmsg_dst, rpmsg_src, rpmsg_payload, 4) != PRU_RPMSG_SUCCESS) {
  }
}
#endif
