// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "rpmsg.h"
#include "gtest/gtest.h"

#include <thread>

TEST(RpmsgTest, StartStop) {
  NewTestTransport();
}

TEST(RpmsgTest, ReadWrite) {
  const std::string expect = "abcdefg";

  auto t = NewTestTransport();

  std::thread hthread([t, expect] {
    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(0, HostRecv(t, &buf, &blen));
    EXPECT_EQ(expect, std::string(buf, blen));

    HostSend(t, static_cast<const void *>(expect.c_str()), static_cast<uint16_t>(expect.size()));
  });

  std::thread cthread([expect] {
    EXPECT_EQ(
        0, ClientSend(&__transport, static_cast<const void *>(expect.c_str()), static_cast<uint16_t>(expect.size())));

    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(0, ClientRecv(&__transport, &buf, &blen));
    EXPECT_EQ(expect, std::string(buf, blen));
  });

  cthread.join();
  hthread.join();
}

TEST(RpmsgTest, ReceiveTransient) {
  auto t = NewTestTransport();

  std::thread hthread([t] { HostTransientReceiveError(t); });

  std::thread cthread([] {
    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(-1, ClientRecv(&__transport, &buf, &blen));
  });

  cthread.join();
  hthread.join();
}

TEST(RpmsgTest, SendTransient) {
  auto t = NewTestTransport();

  std::thread hthread([t] { HostTransientSendError(t); });

  std::thread cthread([] {
    char     buf[32];
    uint16_t blen = 32;
    EXPECT_EQ(-1, ClientSend(&__transport, buf, blen));
  });

  cthread.join();
  hthread.join();
}
