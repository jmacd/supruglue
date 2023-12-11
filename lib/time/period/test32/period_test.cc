// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "lib/coroutine/coroutine.h"
#include "lib/log/daemon/daemon.h"
#include "lib/log/fmt/fmt.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/clock/clock.h"
#include "gtest/gtest.h"

#include <thread>
#include <unordered_set>

// void test_write_func(ThreadID tid, Args args) {
//   int32_t cnt = Atoi(args.ptr);
//   for (int32_t i = 0; i < cnt; i++) {
//     PRULOG_2U(INFO_BLOCK, "write %u", i, 0); // Logs always yield
//     Sleep(10);
//   }
// }

// SUPRUGLUE_DEFINE_THREAD(writer, 500);
// SUPRUGLUE_DEFINE_THREAD(syslog, 500);

// TEST(ClockTest, SleepWake) {
//   auto tt = NewTestTransport();

//   EXPECT_EQ(0, Init(NewSystemConfig()));

//   EXPECT_EQ(0, SystemOnChipIsShutdown());

//   ClockInit();

//   EXPECT_EQ(0, Create(&writer.thread, test_write_func, Args{.ptr = "100"}, "writer", sizeof(writer.space)));
//   EXPECT_EQ(0, Create(&syslog.thread, SyslogProcess, Args{.ptr = ""}, "syslog", sizeof(syslog.space)));

//   std::unordered_set<std::string> res;
//   std::unordered_set<std::string> expect;

//   for (int i = 0; i < 100; i++) {
//     expect.insert(absl::StrFormat("[writer] write %u", i));
//   }

//   int howmany = 0;

//   std::thread client([tt, &res, &howmany] {
//     // read until we receive the correct number, w/o overflow
//     for (int got = 0; got < 100;) {
//       Entry    entry;
//       uint16_t blen = sizeof(entry);
//       EXPECT_EQ(0, HostRecv(tt, &entry, &blen));
//       EXPECT_EQ(blen, sizeof(entry));
//       howmany++;
//       EXPECT_NE(overflowMessage, entry.msg);
//       res.insert(Format(&entry));
//       got += 1;
//     }
//     SystemOnChipShutdown();
//   });

//   EXPECT_EQ(0, ::Run());

//   client.join();

//   // Make sure the logs we received were all expected
//   for (auto arg : res) {
//     EXPECT_NE(expect.end(), expect.find(arg));
//   }
// }
