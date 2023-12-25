// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/soc/test32/soc.h"
#include "absl/synchronization/mutex.h"

absl::Mutex __system_lock;
int         __system_enabled;

void SystemOnChipSetup() {
  absl::MutexLock lock(&__system_lock);
  __system_enabled = 1;
}

void SystemOnChipDelay(int32_t cycles) {
}

void SystemOnChipSleep(void) {
}

int SystemOnChipIsShutdown(void) {
  absl::MutexLock lock(&__system_lock);
  return !__system_enabled;
}

void SystemOnChipShutdown(void) {
  absl::MutexLock lock(&__system_lock);
  __system_enabled = 0;
}
