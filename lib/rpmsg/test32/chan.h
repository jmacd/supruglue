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
