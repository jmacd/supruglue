// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/rpmsg/rpmsg_iface.h"
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
  StartTestTransport(t);

  std::thread host([t, expect] {
    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(0, HostRecv(t, &buf, &blen));
    EXPECT_EQ(expect, std::string(buf, blen));

    EXPECT_EQ(0, HostSend(t, static_cast<const void *>(expect.c_str()), static_cast<uint16_t>(expect.size())));
  });

  std::thread client([expect] {
    EXPECT_EQ(0,
              ClientSend(__transport, static_cast<const void *>(expect.c_str()), static_cast<uint16_t>(expect.size())));

    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(0, ClientRecv(__transport, &buf, &blen));
    EXPECT_EQ(expect, std::string(buf, blen));
  });

  client.join();
  host.join();

  StopTestTransport(t);
}
