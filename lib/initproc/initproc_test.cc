// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "initproc.h"
#include "lib/coroutine/coroutine.h"
#include "lib/intc/service.h"
#include "lib/log/daemon/daemon.h"
#include "lib/log/journal/journal.h"
#include "lib/rpmsg/rpmsg.h"
#include "lib/time/process.h"
#include "gtest/gtest.h"

#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

SUPRUGLUE_DEFINE_THREAD(init, 500);
SUPRUGLUE_DEFINE_THREAD(syslog, 500);

using std::make_pair;
using std::pair;
using std::thread;
using std::unordered_map;
using std::vector;

TEST(InitProc, Simple) {
  auto tt = NewTestTransport();

  EXPECT_EQ(0, Init(NewSystemConfig()));

  InterruptServiceInit();
  ClockInit();

  EXPECT_EQ(0, Create(&init.thread, InitProcess, Args{.ptr = ""}, "init", sizeof(init.space)));
  EXPECT_EQ(0, Create(&syslog.thread, SyslogProcess, Args{.ptr = ""}, "syslog", sizeof(syslog.space)));

  // Count all threads
  int thread_count = 0;
  for (Thread *th = __system.allthreads; th != NULL; th = th->allthreads) {
    thread_count++;
  }

  unordered_map<ThreadID, vector<pair<uint64_t, uint64_t>>> data;

  const int expectRounds = 3;

  thread client([tt, thread_count, &data] {
    // Force it to sleep once.
    HostTransientReceiveError(tt);
    // Wake it up.
    const char *msg = "wakeup";
    EXPECT_EQ(0, HostSend(tt, msg, strlen(msg)));

    int round;
    int n;

    for (round = 0; round < expectRounds; round++) {
      // expect one metric event per thread.
      for (n = 0; n < thread_count;) {
        Entry    entry;
        uint16_t blen = sizeof(entry);
        EXPECT_EQ(0, HostRecv(tt, &entry, &blen));
        EXPECT_EQ(blen, sizeof(entry));

        if ((entry.flags & JW_METRICS) == 0) {
          continue;
        }

        data[entry.tid].emplace_back(make_pair(entry.int1.U64, entry.int2.U64));
        n++;
      }
    }

    Shutdown();
  });

  EXPECT_EQ(0, ::Run());

  client.join();

  for (auto &kv : data) {
    auto &v = kv.second;

    // Less-than because every thread has to run for this test to make progress.
    // The count is nanoseconds: the constant ensures the count starts from 0.
    EXPECT_LT(v[0].first, 1000000000);
    EXPECT_LT(v[0].first, v[1].first);
    EXPECT_LT(v[1].first, v[2].first);

    EXPECT_EQ(v[0].first, v[0].second);
    EXPECT_EQ(v[1].first, v[1].second);
    EXPECT_EQ(v[2].first, v[2].second);
  }
}
