// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/sync/sync.h"
#include "gtest/gtest.h"

TEST(Sync, Init) {
  // Mainly testing linker errors, like happen w/ Bazel 7.0.0 on macOS
  LockWord lock;
  LockInit(&lock);
}
