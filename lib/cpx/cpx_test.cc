// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/cpx.h"
#include "lib/cpx/cpx_test_c.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <setjmp.h>
#include <unordered_map>

using std::string;
using std::vector;

using testing::HasSubstr;

typedef struct {
  Thread    thread;
  uint32_t  index;
  uint32_t *regs;
} TestThread1;

typedef struct {
  Thread         thread;
  vector<string> messages;
} TestThread2;

vector<ThreadID> test_run_get(void) {
  vector<ThreadID> result;
  for (int i = 0; i < test_run_vector_size; i++) {
    result.push_back(test_run_vector[i]);
  }
  return result;
}

string test_logs_format(LogEntry entry) {
  // sigh. use absl?
  string result;
  char   buf[1024];
  snprintf(buf, sizeof(buf), "[%s] ", ((Thread *)entry.tid)->cfg.name);
  result += buf;
  snprintf(buf, sizeof(buf), entry.msg, entry.arg1, entry.arg2);
  result += buf;
  return result;
}

vector<string> test_logs_get() {
  vector<string> result;

  // TODO: not called from a thread, test only.  Cannot yield or
  // block, etc.  Replace me!  See test_read_func.

  while (channelAvailable(&__system.log.ch) != 0) {
    LogEntry ent;
    journalRead(&ent);
    result.push_back(test_logs_format(ent));
  }
  return result;
}

void test_write_func(ThreadID tid, Args args) {
  int32_t cnt = Atoi(args.ptr);
  for (; cnt > 0; cnt--) {
    journal2u("lalala", 0, 0);
  }
}

void test_read_func(ThreadID tid, Args args) {
  TestThread2 *tt2 = (TestThread2 *)tid;
  int32_t      cnt = Atoi(args.ptr);
  for (; cnt > 0; cnt--) {
    LogEntry ent;
    journalRead(&ent);
    tt2->messages.push_back(test_logs_format(ent));
  }
}

void journalBogus(void) {
  LogEntry toomany[LOG_CHANNEL_ENTRIES + 1];
  channelWrite(&__system.log.ch, sizeof(__system.log.space), &toomany, sizeof(toomany));
}

TEST(ArgsTest, Atoi) {
  EXPECT_EQ(0, Atoi("0"));
  EXPECT_EQ(0, Atoi("0 "));
  EXPECT_EQ(0, Atoi("0 h"));
  EXPECT_EQ(1, Atoi("1"));
  EXPECT_EQ(10, Atoi("10"));
  EXPECT_EQ(15, Atoi("15"));
  EXPECT_EQ(1501, Atoi("1501"));
  EXPECT_EQ(1501010, Atoi("1501010 and more"));
  EXPECT_EQ(15, Atoi("15 12 10"));
}

TEST(CpxTest, TwoThreads) {
  TestThread1 threads[2];
  uint32_t    regs[2] = {0, 0};

  threads[0].index = 0;
  threads[1].index = 1;
  threads[0].regs = regs;
  threads[1].regs = regs;

  uint8_t stack0[1024];
  uint8_t stack1[1024];

  auto test_func = [](ThreadID tid, Args args) {
    TestThread1 *tt = (TestThread1 *)tid;
    tt->regs[tt->index] = tt->index + 1;
  };

  EXPECT_EQ(0, Init(NewSystemConfig()));

  EXPECT_EQ(0, Create(&threads[0].thread, test_func, Args{.ptr = "1"}, NewThreadConfig("a", stack0, 1024)));
  EXPECT_EQ(0, Create(&threads[1].thread, test_func, Args{.ptr = "2"}, NewThreadConfig("b", stack1, 1024)));

  EXPECT_EQ(0, ::Run());
  EXPECT_EQ(1, regs[0]);
  EXPECT_EQ(2, regs[1]);
}

TEST(CpxTest, TwiceAlternating) {
  Thread  threads[2];
  uint8_t stack0[1024];
  uint8_t stack1[1024];

  EXPECT_EQ(0, Init(NewSystemConfig()));

  EXPECT_EQ(0, Create(&threads[0], test_run_func, Args{.ptr = "2"}, NewThreadConfig("a", stack0, 1024)));
  EXPECT_EQ(0, Create(&threads[1], test_run_func, Args{.ptr = "22"}, NewThreadConfig("b", stack1, 1024)));

  EXPECT_EQ(0, ::Run());

  auto id0 = TID(&threads[0]);
  auto id1 = TID(&threads[1]);

  vector<ThreadID> expect = {
      id0,
      id1,
      id0,
      id1,
  };

  EXPECT_EQ(expect, test_run_get());
}

TEST(CpxTest, Overflow) {
  Thread  thread;
  uint8_t stack0[500];
  int     num = 1;

  for (; num < 10; num++) {
    EXPECT_EQ(0, Init(NewSystemConfig()));

    char buf[2];
    buf[0] = '0' + num;
    buf[1] = 0;

    EXPECT_EQ(0,
              Create(&thread, test_recursive_func, Args{.ptr = buf}, NewThreadConfig("abc", stack0, sizeof(stack0))));
    EXPECT_EQ(0, ::Run());

    auto logs = test_logs_get();
    if (logs.size() == 0) {
      continue;
    }
    EXPECT_EQ(1, logs.size());
    EXPECT_THAT(logs[0], HasSubstr("[abc] stack overflow"));
    break;
  }
  EXPECT_LT(1, num);
  EXPECT_GT(10, num);
}

TEST(CpxTest, MultiOverflow) {
  Thread  thread0;
  Thread  thread1;
  uint8_t stack0[500];
  uint8_t stack1[500];

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, Create(&thread0, test_recursive_func, Args{.ptr = "9"}, NewThreadConfig("a", stack0, sizeof(stack0))));
  EXPECT_EQ(0, Create(&thread1, test_recursive_func, Args{.ptr = "9"}, NewThreadConfig("b", stack1, sizeof(stack1))));
  EXPECT_EQ(0, ::Run());

  auto logs = test_logs_get();
  EXPECT_EQ(2, logs.size());
  EXPECT_THAT(logs[0], HasSubstr("[a] stack overflow"));
  EXPECT_THAT(logs[1], HasSubstr("[b] stack overflow"));
}

TEST(CpxTest, WriteSizeError) {
  Thread  thread;
  uint8_t stack[500];

  auto test_func = [](ThreadID tid, Args args) { journalBogus(); };

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, Create(&thread, test_func, Args{.ptr = ""}, NewThreadConfig("inval", stack, sizeof(stack))));
  EXPECT_EQ(0, ::Run());

  auto logs = test_logs_get();
  EXPECT_EQ(1, logs.size());
  EXPECT_THAT(logs[0], HasSubstr("[inval] write too large"));
}

TEST(CpxTest, BlockingRead) {
  Thread      th0;
  Thread      th1;
  TestThread2 th2;
  uint8_t     stack0[500];
  uint8_t     stack1[500];
  uint8_t     stack2[500];

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, Create(&th0, test_write_func, Args{.ptr = "100"}, NewThreadConfig("write0", stack0, sizeof(stack0))));
  EXPECT_EQ(0, Create(&th1, test_write_func, Args{.ptr = "100"}, NewThreadConfig("write1", stack1, sizeof(stack1))));
  EXPECT_EQ(0,
            Create(&th2.thread, test_read_func, Args{.ptr = "200"}, NewThreadConfig("reader", stack2, sizeof(stack2))));

  EXPECT_EQ(0, ::Run());

  std::unordered_map<string, int> counts;
  std::unordered_map<string, int> expect;
  expect["[write0] lalala"] = 100;
  expect["[write1] lalala"] = 100;

  auto logs = th2.messages;
  EXPECT_EQ(200, logs.size());
  for (int i = 0; i < 200; i++) {
    counts[logs[i]]++;
  }
  EXPECT_EQ(expect, counts);
}
