# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    hdrs = [
        "clock-am335x.h",
    ],
    srcs = [
        "clock-am335x.c",
    ],
    deps = [
        "//lib/thread",
        "//lib/time/clock:defs",
        "//tools/toolchain/am335x",
    ],
)
