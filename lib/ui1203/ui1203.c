// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "ui1203.h"

UI1203_Reader NewReader(gpio_pin clock, gpio_pin data) {
  UI1203_Reader reader = {
      .clock = clock,
      .data = data,
  };
  return reader;
}
