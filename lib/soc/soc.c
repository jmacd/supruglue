// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "soc.h"

#if SUPRUGLUE_PRU_NUM == 0
#pragma DATA_SECTION(__system_shutdown, ".pru_num.0")
#elif SUPRUGLUE_PRU_NUM == 1
#pragma DATA_SECTION(__system_shutdown, ".pru_num.1")
#endif
int __system_shutdown;
