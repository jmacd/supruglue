// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#ifndef LIB_RPMSG_TEST32_RPMSG_TEST32_CHAN_H
#define LIB_RPMSG_TEST32_RPMSG_TEST32_CHAN_H

#include "absl/synchronization/mutex.h"

template <class T> class Channel {
public:
  Channel(){};
  virtual ~Channel(){};

protected:
  Channel &operator=(const Channel &other) = delete;
  Channel(const Channel &other) = delete;

public:
  T receive() {
    absl::MutexLock lock(&_lock);
    _has_receiver = true;
    _lock.Await(absl::Condition(
        +[](Channel<T> *ch) { return ch->_has_receiver && ch->_has_value; }, this));
    _has_receiver = false;
    _has_value = false;
    return std::move(_val);
  };

  void send(T &&val) {
    absl::MutexLock lock(&_lock);
    _lock.Await(absl::Condition(
        +[](Channel<T> *ch) { return (ch->_has_receiver && !ch->_has_value); }, this));
    _val = val;
    _has_value = true;
  };

  void close() {
  }

protected:
  T           _val;
  bool        _has_value{false};
  bool        _has_receiver{false};
  absl::Mutex _lock;
};

#endif // LIB_RPMSG_TEST32_RPMSG_TEST32_CHAN_H
