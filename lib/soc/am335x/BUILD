# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    hdrs = [
        "soc.h",
    ],
    srcs = [
        "soc.c",
        "sleep.s",
    ],
    deps = [
        "//lib/soc:defs",

        "//tools/toolchain/am335x",
    ],
)
