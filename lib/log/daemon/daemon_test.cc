// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "daemon.h"

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "lib/coroutine/coroutine.h"
#include "lib/intc/service.h"
#include "lib/log/fmt/fmt.h"
#include "lib/log/journal/journal.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/process.h"
#include "gtest/gtest.h"

#include <thread>
#include <unordered_set>

void test_write_func(ThreadID tid, Args args) {
  int32_t cnt = Atoi(args.ptr);
  for (int32_t i = 0; i < cnt; i++) {
    PRULOG_1u32(INFO, "write %u", i); // Logs always yield
  }
}

SUPRUGLUE_DEFINE_THREAD(writer0, 500);
SUPRUGLUE_DEFINE_THREAD(writer1, 500);

TEST(Syslog, Simple) {
  auto tt = NewTestTransport();

  EXPECT_EQ(0, Init(NewSystemConfig()));

  InterruptServiceInit();
  ClockInit();
  SyslogInit();

  EXPECT_EQ(0, Create(&writer0.thread, test_write_func, Args{.ptr = "100"}, "writer0", sizeof(writer0.space)));
  EXPECT_EQ(0, Create(&writer1.thread, test_write_func, Args{.ptr = "100"}, "writer1", sizeof(writer1.space)));

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
        overflowed += entry.int1.U32.LOW;
        got += entry.int1.U32.LOW;
      } else {
        res.insert(Format(&entry));
        got += 1;
      }
    }
    Shutdown();
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
  InterruptServiceInit();
  ClockInit();
  SyslogInit();

  EXPECT_EQ(0, Create(&writer0.thread, test_write_func, Args{.ptr = "1"}, "writer0", sizeof(writer0.space)));

  std::thread client([tt] {
    // Some transients.
    HostTransientSendError(tt);
    HostTransientSendError(tt);
    HostTransientSendError(tt);

    Entry    entry;
    uint16_t blen = sizeof(entry);

    EXPECT_EQ(0, HostRecv(tt, &entry, &blen));
    EXPECT_EQ(blen, sizeof(entry));
    EXPECT_EQ("[writer0] write 0", Format(&entry));

    Shutdown();
  });

  EXPECT_EQ(0, ::Run());

  client.join();
}
