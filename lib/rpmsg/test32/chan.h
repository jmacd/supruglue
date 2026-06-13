// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_TEST32_RPMSG_TEST32_CHAN_H
#define LIB_RPMSG_TEST32_RPMSG_TEST32_CHAN_H

#include "absl/synchronization/mutex.h"
#include "lib/soc/soc.h"
#include <optional>

extern absl::Mutex __system_lock;

template <class T> class Channel {
public:
  Channel(){};
  virtual ~Channel(){};

protected:
  Channel &operator=(const Channel &other) = delete;
  Channel(const Channel &other) = delete;

public:
  // Non-blocking send used from the cooperative PRU scheduler, which must
  // never block the scheduler thread on the host. Returns 0 if the value was
  // delivered to a waiting receiver, or -1 if no receiver is ready yet or an
  // error was injected (mirrors the am335x pru_rpmsg_send retry contract).
  int try_send(T &&val) {
    absl::MutexLock lock(&__system_lock);
    if (__system_shutdown) {
      return -1;
    }
    if (!(_has_receiver && !_has_value)) {
      return -1;
    }
    _has_value = true;
    if (_has_error) {
      _val.reset();
      return -1;
    }
    _val = val;
    return 0;
  }

  // Non-blocking receive used from the cooperative PRU scheduler. Returns a
  // value when one is available (which may be empty if an error was injected),
  // or nullopt if nothing is ready yet so the caller can yield and retry.
  std::optional<T> try_receive() {
    absl::MutexLock lock(&__system_lock);
    _has_receiver = true;
    if (!_has_value) {
      return std::nullopt;
    }
    _has_receiver = false;
    _has_value = false;
    return std::move(_val);
  }

  // causes opposite send() to get a value
  std::optional<T> receive() {
    absl::MutexLock lock(&__system_lock);
    _has_receiver = true;
    __system_lock.Await(absl::Condition(
        +[](Channel<T> *ch) { return __system_shutdown || (ch->_has_receiver && ch->_has_value); }, this));
    _has_receiver = false;
    _has_value = false;
    return std::move(_val);
  };

  // causes opposite send() to get an error
  void sender_transient() {
    absl::MutexLock lock(&__system_lock);
    _has_receiver = true;
    _has_error = true;
    __system_lock.Await(absl::Condition(
        +[](Channel<T> *ch) { return __system_shutdown || (ch->_has_receiver && ch->_has_value); }, this));
    _has_receiver = false;
    _has_error = false;
    _has_value = false;
  }

  // causes opposite receive() to get a value
  int send(T &&val) {
    absl::MutexLock lock(&__system_lock);
    __system_lock.Await(absl::Condition(
        +[](Channel<T> *ch) { return __system_shutdown || (ch->_has_receiver && !ch->_has_value); }, this));
    _has_value = true;
    if (__system_shutdown || _has_error) {
      _val.reset();
      return -1;
    }
    _val = val;
    return 0;
  };

  // causes opposite receive() to get an error
  void receiver_transient() {
    absl::MutexLock lock(&__system_lock);
    __system_lock.Await(absl::Condition(
        +[](Channel<T> *ch) { return __system_shutdown || (ch->_has_receiver && !ch->_has_value); }, this));
    _val.reset();
    _has_value = true;
  }

protected:
  std::optional<T> _val;
  bool             _has_value{false};
  bool             _has_receiver{false};
  bool             _has_error{false};
};

#endif // LIB_RPMSG_TEST32_RPMSG_TEST32_CHAN_H
