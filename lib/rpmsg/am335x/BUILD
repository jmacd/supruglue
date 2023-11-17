# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "rpmsg.c",
    ],
    hdrs = [
        "rpmsg.h",
    ],
    deps = [
        "//lib/rpmsg:defs",
        "//lib/soc",
        "//lib/debug",
        "//lib/thread",
        "//lib/intc",
        "//tools/toolchain/am335x",
        "@ti-pru-support//lib/src/rpmsg_lib",
    ],
)
