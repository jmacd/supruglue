// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/rpmsg/rpmsg_test32.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/synchronization/mutex.h"
#include <thread>

struct _RpmsgTransport {
  _RpmsgTransport() : thread(&_RpmsgTransport::run, this) {
  }

  absl::Mutex lock;
  std::thread thread;

  void run() {
    LOG(INFO) << "hello";
  }
};

RpmsgTransport *NewRpmsgTransport(void) {
  return new RpmsgTransport;
}

void StartTransport(RpmsgTransport *rpmt) {
}

void StopTransport(RpmsgTransport *rpmt) {
  rpmt->thread.join();
}

// TODO: @@@ this does not belong here
// struct Local {};
// Local init() {
//   absl::InitializeLog();
//   return Local{};
// }
// static Local __ = init();
