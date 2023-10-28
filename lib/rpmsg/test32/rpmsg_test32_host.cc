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

int ClientSend(ClientTransport *transport, const void *data, uint16_t len) {
  return transport->test->pru_to_host.send(std::string(static_cast<const char *>(data), len));
}

int ClientRecv(ClientTransport *transport, void *data, uint16_t *len) {
  std::optional<std::string> r = transport->test->host_to_pru.receive();
  if (!r) {
    return -1;
  }
  // TODO: should test that *len can hold r.size()?
  memcpy(data, r.value().c_str(), r.value().size());
  *len = r.value().size();
  return 0;
}

int HostRecv(TestTransport *transport, void *data, uint16_t *len) {
  std::optional<std::string> r = transport->pru_to_host.receive();
  if (!r) {
    return -1;
  }
  // TODO: should test that *len can hold r.size()?
  memcpy(data, r.value().c_str(), r.value().size());
  *len = r.value().size();
  return 0;
}

int HostSend(TestTransport *transport, const void *data, uint16_t len) {
  return transport->host_to_pru.send(std::string(static_cast<const char *>(data), len));
}

void HostTransientReceiveError(TestTransport *transport) {
  transport->host_to_pru.receiver_transient();
}

void HostTransientSendError(TestTransport *transport) {
  transport->pru_to_host.sender_transient();
}
