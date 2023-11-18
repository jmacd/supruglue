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
        "//lib/debug",
        "//lib/intc",
        "//lib/rpmsg:defs",
        "//lib/soc",
        "//lib/thread",
        "//tools/toolchain/am335x",
        "@ti-pru-support//lib/src/rpmsg_lib",
    ],
)
