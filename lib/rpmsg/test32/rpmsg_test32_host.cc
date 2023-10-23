// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "rpmsg_test32_host.h"
#include "rpmsg_test32_chan.h"
#include <thread>

ClientTransport *__transport;

struct _ClientTransport {
  Channel<std::string> host_to_pru;
  Channel<std::string> pru_to_host;
};

struct _TestTransport {
  _TestTransport() {
  }

  ClientTransport client;
};

TestTransport *NewTestTransport(void) {
  return new TestTransport;
}

void StartTestTransport(TestTransport *rpmt) {
  __transport = &rpmt->client;
}

void StopTestTransport(TestTransport *rpmt) {
  __transport = nullptr;
}

int ClientSend(ClientTransport *transport, const void *data, uint16_t len) {
  transport->pru_to_host.send(std::string(static_cast<const char *>(data), len));
  return 0;
}

int ClientRecv(ClientTransport *transport, void *data, uint16_t *len) {
  std::string r = transport->host_to_pru.receive();
  // TODO: should test that *len can hold r.size()?
  memcpy(data, r.c_str(), r.size());
  *len = r.size();
  return 0;
}

int HostSend(TestTransport *transport, const void *data, uint16_t len) {
  transport->client.host_to_pru.send(std::string(static_cast<const char *>(data), len));
  return 0;
}

int HostRecv(TestTransport *transport, void *data, uint16_t *len) {
  std::string r = transport->client.pru_to_host.receive();
  // TODO: should test that *len can hold r.size()?
  memcpy(data, r.c_str(), r.size());
  *len = r.size();
  return 0;
}
