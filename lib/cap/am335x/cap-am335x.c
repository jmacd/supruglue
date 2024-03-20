// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "cap-am335x.h"
#include "external/ti-pru-support/include/am335x/pru_ecap.h"

void CAP_Init(void) {
  CT_ECAP.ECCLR = 0xFF;

  CT_ECAP.ECCTL1 = (3 << 14) | // Free run
                   (1 << 8) |  // Capture enable
                   (0 << 7) |  // Reset after 4
                   (0 << 6) |  // Cap4 polarity falling
                   (0 << 5) |  // Reset after 3
                   (0 << 4) |  // Cap3 polarity rising
                   (1 << 3) |  // Reset after 2
                   (1 << 2) |  // Cap2 polarity falling
                   (1 << 1) |  // Reset after 1
                   (0 << 0);   // Cap1 polarity rising

  CT_ECAP.ECCTL2 = (0 << 9) | // Capture mode
                   (0 << 6) | // Sync out disable
                   (0 << 5) | // Sync in disable
                   (0 << 4) | // Stopped
                   (1 << 1) | // Capture 2 events
                   (0 << 0);  // Continuous

  CT_ECAP.ECEINT = (1 << 2); // Interrupt on Cap2
}

void CAP_Enable(void) {
  CT_ECAP.ECCTL2 |= (1 << 4); // Run
}

void CAP_ClearInterrupt(void) {
  CT_ECAP.ECCLR = (1 << 0) | // INTR
                  (1 << 1) | // CEVT1
                  (1 << 2) | // CEVT2
                  (1 << 5);  // OFLOW
}
