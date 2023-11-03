// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "daemon.h"

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "lib/log/fmt/fmt.h"
#include "lib/rpmsg/test32/rpmsg_test32_host.h"
#include "gtest/gtest.h"

#include <thread>
#include <unordered_set>

void test_write_func(ThreadID tid, Args args) {
  int32_t cnt = Atoi(args.ptr);
  for (int32_t i = 0; i < cnt; i++) {
    PRULOG_2U(INFO, "write %u", i, 0); // Logs always yield
  }
}

TEST(Syslog, Simple) {
  auto tt = NewTestTransport();

  EXPECT_EQ(0, Init(NewSystemConfig()));

  Thread  writer0;
  Thread  writer1;
  Thread  syslog;
  uint8_t stack0[500];
  uint8_t stack1[500];
  uint8_t stack2[500];

  EXPECT_EQ(0,
            Create(&writer0, test_write_func, Args{.ptr = "100"}, NewThreadConfig("writer0", stack0, sizeof(stack0))));
  EXPECT_EQ(0,
            Create(&writer1, test_write_func, Args{.ptr = "100"}, NewThreadConfig("writer1", stack1, sizeof(stack1))));
  EXPECT_EQ(0, Create(&syslog, SyslogProcess, Args{.ptr = ""}, NewThreadConfig("syslog", stack2, sizeof(stack2))));

  std::unordered_set<std::string> res;
  std::unordered_set<std::string> expect;

  for (int i = 0; i < 100; i++) {
    expect.insert(absl::StrFormat("[writer0] write %u", i));
    expect.insert(absl::StrFormat("[writer1] write %u", i));
  }

  int howmany = 0;
  int overflowed = 0;
  int overflows = 0;

  std::thread client([tt, &res, &howmany, &overflows, &overflowed] {
    // read until we receive the correct number, counting skips
    for (int got = 0; got < 200;) {
      Entry    entry;
      uint16_t blen = sizeof(entry);
      EXPECT_EQ(0, HostRecv(tt, &entry, &blen));
      EXPECT_EQ(blen, sizeof(entry));
      howmany++;
      if (entry.msg == overflowMessage) {
        overflows++;
        overflowed += entry.arg1;
        got += entry.arg1;
      } else {
        res.insert(Format(&entry));
        got += 1;
      }
    }
  });

  EXPECT_EQ(0, ::Run());

  client.join();

  // The number of overflows and overflowed counts are logical.
  EXPECT_EQ(overflowed + howmany - 200, overflows);

  // Make sure the logs we received were all expected
  for (auto arg : res) {
    EXPECT_NE(expect.end(), expect.find(arg));
  }
}

TEST(Syslog, WithTransients) {
  auto tt = NewTestTransport();

  EXPECT_EQ(0, Init(NewSystemConfig()));

  Thread  writer;
  Thread  syslog;
  uint8_t stack0[500];
  uint8_t stack1[500];

  EXPECT_EQ(0, Create(&writer, test_write_func, Args{.ptr = "1"}, NewThreadConfig("writer", stack0, sizeof(stack0))));
  EXPECT_EQ(0, Create(&syslog, SyslogProcess, Args{.ptr = ""}, NewThreadConfig("syslog", stack1, sizeof(stack1))));

  std::thread client([tt] {
    // Some transients.
    HostTransientSendError(tt);
    HostTransientSendError(tt);
    HostTransientSendError(tt);

    Entry    entry;
    uint16_t blen = sizeof(entry);

    EXPECT_EQ(0, HostRecv(tt, &entry, &blen));
    EXPECT_EQ(blen, sizeof(entry));
    EXPECT_EQ("[writer] write 0", Format(&entry));
  });

  EXPECT_EQ(0, ::Run());

  client.join();
}
