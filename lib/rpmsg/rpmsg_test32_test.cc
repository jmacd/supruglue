// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/rpmsg/rpmsg_test32.h"
#include "gtest/gtest.h"

TEST(RpmsgTest, Basic) {
  auto t = NewRpmsgTransport();
  StartTransport(t);
  StopTransport(t);
}
