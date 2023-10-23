// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "lib/cpx/fmt.h"
#include "absl/strings/str_format.h"
#include "lib/cpx/cpx.h"

std::string Format(LogEntry *entry) {
  std::string hdr = absl::StrFormat("[%s] ", ((Thread *)entry->tid)->cfg.name);

  auto fmt2str = absl::ParsedFormat<'u', 'u'>::New(entry->msg);
  if (fmt2str) {
    std::string res = absl::StrFormat(*fmt2str, entry->arg1, entry->arg2);
    return hdr + res;
  }
  auto fmt1str = absl::ParsedFormat<'u'>::New(entry->msg);
  if (fmt1str) {
    std::string res = absl::StrFormat(*fmt1str, entry->arg1);
    return hdr + res;
  }
  // TODO shrug. this library is too strict! ok.
  return "invalid!";
}
