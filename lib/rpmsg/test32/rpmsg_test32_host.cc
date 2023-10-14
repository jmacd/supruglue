// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "rpmsg_test32_host.h"
#include "absl/log/log.h"
#include "absl/synchronization/mutex.h"
#include <thread>

struct _ClientTransport {};

struct _TestTransport {
  _TestTransport() : thread(&_TestTransport::run, this) {
  }

  absl::Mutex     lock;
  std::thread     thread;
  ClientTransport client;

  void run() {
    LOG(INFO) << "hello";
  }
};

TestTransport *NewTestTransport(void) {
  return new TestTransport;
}

ClientTransport *GetClientTransport(TestTransport *host) {
  return &host->client;
}

void StartTestTransport(TestTransport *rpmt) {
}

void StopTestTransport(TestTransport *rpmt) {
  rpmt->thread.join();
}

int ClientSend(ClientTransport *transport, const void *data, uint16_t len) {
  return 0;
}

int ClientRecv(ClientTransport *transport, void *data, uint16_t *len) {
  return 0;
}

int HostSend(TestTransport *transport, const void *data, uint16_t len) {
  return 0;
}

int HostRecv(TestTransport *transport, void *data, uint16_t *len) {
  return 0;
}
