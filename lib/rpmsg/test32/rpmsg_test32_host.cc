// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "rpmsg_test32_host.h"
#include "rpmsg_test32_chan.h"
#include <thread>

struct _TestTransport {
  Channel<std::string> host_to_pru;
  Channel<std::string> pru_to_host;
};

struct _ClientTransport {
  TestTransport *test;
};

ClientTransport __transport;

TestTransport *NewTestTransport(void) {
  __transport.test = new TestTransport;
  return __transport.test;
}

void StartTestTransport(TestTransport *rpmt) {
}

int ClientSend(ClientTransport *transport, const void *data, uint16_t len) {
  transport->test->pru_to_host.send(std::string(static_cast<const char *>(data), len));
  return 0;
}

int ClientRecv(ClientTransport *transport, void *data, uint16_t *len) {
  std::string r = transport->test->host_to_pru.receive();
  // TODO: should test that *len can hold r.size()?
  memcpy(data, r.c_str(), r.size());
  *len = r.size();
  return 0;
}

int HostSend(TestTransport *transport, const void *data, uint16_t len) {
  transport->host_to_pru.send(std::string(static_cast<const char *>(data), len));
  return 0;
}

int HostRecv(TestTransport *transport, void *data, uint16_t *len) {
  std::string r = transport->pru_to_host.receive();
  // TODO: should test that *len can hold r.size()?
  memcpy(data, r.c_str(), r.size());
  *len = r.size();
  return 0;
}
