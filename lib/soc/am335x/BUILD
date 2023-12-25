# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "delay.s",
        "sleep.s",
        "soc.c",
    ],
    hdrs = [
        "soc.h",
    ],
    deps = [
        "//lib/soc:defs",
        "//tools/toolchain/am335x",
    ],
)
