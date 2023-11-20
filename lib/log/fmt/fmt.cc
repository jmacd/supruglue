// Copyright Joshua MacDonald
// SPDX-License-Identifier: MIT

#include "fmt.h"
#include "absl/strings/str_format.h"
#include "lib/coroutine/coroutine.h"

std::string Format(Entry *entry) {
  std::string hdr = "[-] ";

  Thread *th = reinterpret_cast<Thread *>(entry->tid);
  if (th != nullptr) {
    hdr = absl::StrFormat("[%s] ", th->name);
  }

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
  // TODO shrug.
  return "invalid!";
}
