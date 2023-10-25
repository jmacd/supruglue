// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_AM335X_SOC_H
#define SUPRUGLUE_AM335X_SOC_H

#define WORDSZ sizeof(uint32_t)

// This macro assumes standard linker commands, i.e., normal
// assignment of the PRU_DMEM_0_1 section to pru0 (0x0000...0x2000) and
// assignment of the PRU_DMEM_1_0 section to pru1 (0x2000...0x4000).
#define PRU_CORE_NUMBER(soc) (((&__system) < 0x2000) ? 0 : 1)

#endif // SUPRUGLUE_AM335X_SOC_H
