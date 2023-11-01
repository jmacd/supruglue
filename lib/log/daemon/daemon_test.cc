// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "absl/strings/str_format.h"
#include "lib/cpx/fmt.h"
#include "lib/rpmsg/test32/rpmsg_test32_host.h"
#include "lib/syslog/syslog.h"
#include "gtest/gtest.h"

#include <thread>
#include <unordered_set>

vector<string> test_logs_get() {
  vector<string> result;

  // TODO: not called from a thread, test only.  Cannot yield or
  // block, etc.  Replace me!  See test_read_func.

  while (channelAvailable(&__system.log.ch, sizeof(__system.log.space)) != 0) {
    LogEntry ent;
    journalRead(&ent);
    result.push_back(Format(&ent));
  }
  return result;
}

void test_write_func(ThreadID tid, Args args) {
  int32_t cnt = Atoi(args.ptr);
  for (int32_t i = 0; i < cnt; i++) {
    journal2u("write %u", i, 0);
    Yield();
  }
}

void test_read_func(ThreadID tid, Args args) {
  TestThread2 *tt2 = (TestThread2 *)tid;

  int cnt = -1;
  if (args.ptr != nullptr) {
    cnt = Atoi(args.ptr);
  }
  for (int32_t i = 0; cnt < 0 || i < cnt; i++) {
    LogEntry ent;
    journalRead(&ent);
    auto rd = Format(&ent);
    tt2->messages.push_back(rd);
    Yield();
  }
}

void journalBogus(void) {
  LogEntry toomany[LOG_CHANNEL_ENTRIES + 1];
  channelWrite(&__system.log.ch, sizeof(__system.log.space), &toomany, sizeof(toomany));
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

  std::thread client([tt, &res] {
    for (int i = 0; i < 200; i++) {
      LogEntry entry;
      uint16_t blen = sizeof(entry);
      EXPECT_EQ(0, HostRecv(tt, &entry, &blen));
      EXPECT_EQ(blen, sizeof(entry));
      res.insert(Format(&entry));
    }
  });

  EXPECT_EQ(0, ::Run());

  client.join();

  EXPECT_EQ(expect, res);
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

    LogEntry entry;
    uint16_t blen = sizeof(entry);

    EXPECT_EQ(0, HostRecv(tt, &entry, &blen));
    EXPECT_EQ(blen, sizeof(entry));
    EXPECT_EQ("[writer] write 0", Format(&entry));
  });

  EXPECT_EQ(0, ::Run());

  client.join();
}
