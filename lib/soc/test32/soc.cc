// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/soc/test32/soc.h"
#include "absl/synchronization/mutex.h"

absl::Mutex __system_lock;

void SystemOnChipSetup() {
}

void SystemOnChipDelay(int32_t cycles) {
}

void SystemOnChipSuspend(void) {
}

void Shutdown(void) {
  absl::MutexLock lock(&__system_lock);
  __system_shutdown = 1;
}
