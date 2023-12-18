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
        "//lib/sync",
        "//lib/intc",
        "//lib/time/clock:defs",
        "//tools/toolchain/am335x",
    ],
)

filegroup(
    name = "iep_tim_cap_cmp_pend",
    srcs = [
        "iep_tim_cap_cmp_pend.yaml",
    ],
)
