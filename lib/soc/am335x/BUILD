# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "delay.s",
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
