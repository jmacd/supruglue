// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef SUPRUGLUE_AM335X_SOC_H
#define SUPRUGLUE_AM335X_SOC_H

#ifdef __cplusplus
extern "C" {
#endif

inline void SystemOnChipSetup() {
}

inline void SystemOnChipTeardown() {
}

// This macro assumes standard linker commands, i.e., normal
// assignment of the PRU_DMEM_0_1 section to pru0 (0x0000...0x2000) and
// assignment of the PRU_DMEM_1_0 section to pru1 (0x2000...0x4000).
#define PRU_CORE_NUMBER() (((&__dummy) < (void *)0x2000) ? 0 : 1)

// Dummy variable for PRU_CORE_NUMBER check.
extern void *__dummy;

#ifdef __cplusplus
}
#endif

#endif // SUPRUGLUE_AM335X_SOC_H
