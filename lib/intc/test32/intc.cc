// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include <string.h>
#include <vector>

#include "absl/log/log.h"
#include "absl/synchronization/mutex.h"
#include "lib/intc/test32/intc.h"

using std::vector;

InterruptController __controller;

struct _TestInterruptController {
  _TestInterruptController() : _pending(NUM_SYSEVTS), _handler(NUM_SYSEVTS) {
  }

  void Service() {
    absl::MutexLock lock(&_lock);
    for (int i = 0; i < NUM_SYSEVTS; i++) {
      if (_pending[i]) {
        _pending[i] = false;
        _handler[i]();
      }
    }
  }

  void Init(uint8_t evt, InterruptHandler *handler) {
    absl::MutexLock lock(&_lock);
    _handler[evt] = handler;
  }

  void Raise(uint8_t evt) {
    absl::MutexLock lock(&_lock);
    _pending[evt] = true;
  }

  absl::Mutex                _lock;
  vector<bool>               _pending;
  vector<InterruptHandler *> _handler;
};

void ControllerInit(void) {
  __controller.test = new TestInterruptController;
}

void ServiceInterrupts(void) {
  LOG(INFO) << "SERVICE";
  __controller.test->Service();
}

void InterruptHandlerInit(uint8_t evt, InterruptHandler *handler) {
  __controller.test->Init(evt, handler);
}

void RaiseInterrupt(uint8_t evt) {
  LOG(INFO) << "RAISE";
  __controller.test->Raise(evt);
}
