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
        _handler[i](_args[i]);
      }
    }
  }

  void Init(uint8_t evt, InterruptHandler *handler, Args args) {
    absl::MutexLock lock(&_lock);
    _handler[evt] = handler;
    _args[evt] = args;
  }

  void Raise(uint8_t evt) {
    absl::MutexLock lock(&_lock);
    _pending[evt] = true;
  }

  absl::Mutex                _lock;
  vector<bool>               _pending;
  vector<InterruptHandler *> _handler;
  vector<Args>               _args;
};

void ControllerInit(void) {
  __controller.test = new TestInterruptController;
}

void ServiceInterrupts(void) {
  __controller.test->Service();
}

void InterruptHandlerInit(uint8_t evt, InterruptHandler *handler, Args args) {
  __controller.test->Init(evt, handler, args);
}

void RaiseInterrupt(uint8_t evt) {
  __controller.test->Raise(evt);
}

void ControllerEnable(void) {
  // @@@
}
