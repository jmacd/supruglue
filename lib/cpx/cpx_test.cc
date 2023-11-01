// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/cpx.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <unordered_map>

using std::string;
using std::vector;

using testing::HasSubstr;

ThreadID test_run_vector[32];
size_t   test_run_vector_size;

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

void test_run_reset(void) {
  memset(test_run_vector, 0, sizeof(test_run_vector));
  test_run_vector_size = 0;
}

void test_run_func(ThreadID tid, Args args) {
  for (int i = 0; i < 2; i++) {
    Yield();
    // This follows the Yield so that we test entry and return to the
    // coroutine.
    test_run_vector[test_run_vector_size++] = tid;
  }
}

void test_recursive_func(ThreadID tid, Args args) {
  const char *arg = TakeArg(&args);

  Yield();
  if (*arg != '0') {
    char two[2];
    two[0] = *arg - 1;
    two[1] = 0;
    Args rargs;
    rargs.ptr = two;
    test_recursive_func(tid, rargs);
  }
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

TEST(CpxTest, OneBlockingRead) {
  Thread      writer;
  TestThread2 reader;
  uint8_t     stack0[500];
  uint8_t     stack1[500];

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, Create(&writer, test_write_func, Args{.ptr = "10"}, NewThreadConfig("write0", stack0, sizeof(stack0))));
  EXPECT_EQ(
      0, Create(&reader.thread, test_read_func, Args{.ptr = "10"}, NewThreadConfig("reader", stack1, sizeof(stack1))));

  EXPECT_EQ(0, ::Run());

  std::unordered_map<string, int> counts;
  std::unordered_map<string, int> expect;

  auto logs = reader.messages;
  EXPECT_EQ(10, logs.size());

  for (int32_t i = 0; i < 10; i++) {
    expect["[write0] lalala " + std::to_string(i)] = 1;
    counts[logs[i]]++;
  }

  EXPECT_EQ(expect, counts);
}

TEST(CpxTest, TwoBlockingWriters) {
  Thread      writer0;
  Thread      writer1;
  TestThread2 reader;
  uint8_t     stack0[500];
  uint8_t     stack1[500];
  uint8_t     stack2[500];

  const int    items = 100;
  const string each = std::to_string(items);
  const string twice = std::to_string(2 * items);

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, Create(&writer0, test_write_func, Args{.ptr = each.c_str()},
                      NewThreadConfig("write0", stack0, sizeof(stack0))));
  EXPECT_EQ(0, Create(&writer1, test_write_func, Args{.ptr = each.c_str()},
                      NewThreadConfig("write1", stack1, sizeof(stack1))));
  EXPECT_EQ(0, Create(&reader.thread, test_read_func, Args{.ptr = twice.c_str()},
                      NewThreadConfig("reader", stack2, sizeof(stack2))));

  EXPECT_EQ(0, ::Run());

  std::unordered_map<string, int> counts;
  std::unordered_map<string, int> expect;

  auto logs = reader.messages;
  EXPECT_EQ(2 * items, logs.size());
  for (int32_t i = 0; i < logs.size(); i++) {
    counts[logs[i]] += 1;
  }

  for (int32_t i = 0; i < items; i++) {
    expect["[write0] lalala " + std::to_string(i)] = 1;
    expect["[write1] lalala " + std::to_string(i)] = 1;
  }

  EXPECT_EQ(expect, counts);
}

TEST(CpxTest, TwoBlockingReaders) {
  Thread      writer;
  TestThread2 reader0;
  TestThread2 reader1;
  uint8_t     stack0[500];
  uint8_t     stack1[500];
  uint8_t     stack2[500];

  const int    items = 10;
  const string each = std::to_string(items);
  const string twice = std::to_string(2 * items);

  EXPECT_EQ(0, Init(NewSystemConfig()));
  EXPECT_EQ(0, Create(&writer, test_write_func, Args{.ptr = twice.c_str()},
                      NewThreadConfig("writer", stack0, sizeof(stack0))));
  EXPECT_EQ(0, Create(&reader0.thread, test_read_func, Args{.ptr = each.c_str()},
                      NewThreadConfig("reader0", stack1, sizeof(stack1))));
  EXPECT_EQ(0, Create(&reader1.thread, test_read_func, Args{.ptr = each.c_str()},
                      NewThreadConfig("reader1", stack2, sizeof(stack2))));

  EXPECT_EQ(0, ::Run());

  std::unordered_map<string, int> counts;
  std::unordered_map<string, int> expect;

  auto logs0 = reader0.messages;
  auto logs1 = reader1.messages;
  EXPECT_EQ(items, logs0.size());
  EXPECT_EQ(items, logs1.size());
  for (int32_t i = 0; i < logs0.size(); i++) {
    counts[logs0[i]] += 1;
  }
  for (int32_t i = 0; i < logs1.size(); i++) {
    counts[logs1[i]] += 1;
  }

  for (int32_t i = 0; i < 2 * items; i++) {
    expect["[writer] lalala " + std::to_string(i)] = 1;
  }

  EXPECT_EQ(expect, counts);
}

TEST(CpxTest, NonBlockingWrite) {
  Thread  writer;
  uint8_t stack[500];

  const int    items = LOG_CHANNEL_ENTRIES + 1;
  const string each = std::to_string(items);

  auto cfg = NewSystemConfig();
  cfg.log_flags = CF_LOGGING;

  EXPECT_EQ(0, Init(cfg));
  EXPECT_EQ(
      0, Create(&writer, test_write_func, Args{.ptr = each.c_str()}, NewThreadConfig("write", stack, sizeof(stack))));

  EXPECT_EQ(0, ::Run());

  vector<string> counts = test_logs_get();
  vector<string> expect;

  EXPECT_EQ(expect, counts);
}
