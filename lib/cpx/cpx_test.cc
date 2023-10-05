// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/cpx.h"
#include "lib/cpx/cpx_test_c.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <setjmp.h>

using std::string;
using std::vector;

using testing::HasSubstr;

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
  int32_t        count = std::min(__system.log.sequence, LOG_QUEUE_SIZE);
  int32_t        idx = __system.log.sequence - count;
  vector<string> result;
  for (; idx < count; idx++) {
    result.push_back(test_logs_format(__system.log.queue[idx % LOG_QUEUE_SIZE]));
  }
  return result;
}

typedef struct {
  Thread    thread;
  uint32_t  index;
  uint32_t *regs;
} TestThread;

TEST(CpxTest, TwoThreads) {
  TestThread threads[2];
  uint32_t   regs[2] = {0, 0};

  threads[0].index = 0;
  threads[1].index = 1;
  threads[0].regs = regs;
  threads[1].regs = regs;

  uint8_t stack0[1024];
  uint8_t stack1[1024];

  auto test_func = [](ThreadID tid, Args args) {
    TestThread *tt = (TestThread *)tid;
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

TEST(CpxTest, TestOverflow) {
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

TEST(CpxTest, TestMultiOverflow) {
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
