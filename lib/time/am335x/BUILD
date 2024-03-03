# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "clock-am335x.c",
    ],
    hdrs = [
        "clock-am335x.h",
    ],
    deps = [
        "//lib/intc",
        "//lib/sync",
        "//lib/thread",
        "//lib/time:defs",
        "//tools/toolchain/am335x",
    ],
)

filegroup(
    name = "iep_tim_cap_cmp_pend",
    srcs = [
        "iep_tim_cap_cmp_pend.yaml",
    ],
)
