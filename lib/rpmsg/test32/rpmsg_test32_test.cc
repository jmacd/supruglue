// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/rpmsg/am335x/rpmsg_am335x_iface.h"
#include "rpmsg_test32_host.h"
#include "gtest/gtest.h"

#include <thread>

TEST(RpmsgTest, StartStop) {
  auto t = NewTestTransport();
  StartTestTransport(t);
  StopTestTransport(t);
}

TEST(RpmsgTest, ReadWrite) {
  const std::string expect = "abcdefg";

  auto t = NewTestTransport();

  std::thread host([t, expect] {
    StartTestTransport(t);

    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(0, HostRecv(t, &buf, &blen));
    EXPECT_EQ(expect, std::string(buf, blen));

    EXPECT_EQ(0, HostSend(t, static_cast<const void *>(expect.c_str()), static_cast<uint16_t>(expect.size())));

    StopTestTransport(t);
  });

  auto c = GetClientTransport(t);

  std::thread client([c, expect] {
    EXPECT_EQ(0, ClientSend(c, static_cast<const void *>(expect.c_str()), static_cast<uint16_t>(expect.size())));

    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(0, ClientRecv(c, &buf, &blen));
    EXPECT_EQ(expect, std::string(buf, blen));
  });

  client.join();
  host.join();
}
