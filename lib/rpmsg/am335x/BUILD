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
        "//lib/sync",
        "//lib/thread",
        "//tools/toolchain/am335x",
        "@ti-pru-support//lib/src/rpmsg_lib",
    ],
)

filegroup(
    name = "pru0_irq",
    srcs = [
        "pru0.yaml",
    ],
)

filegroup(
    name = "pru1_irq",
    srcs = [
        "pru1.yaml",
    ],
)

filegroup(
    name = "irq",
    srcs = select({
        "//tools/toolchain:pru0": [
            "pru0.yaml",
        ],
        "//tools/toolchain:pru1": [
            "pru1.yaml",
        ],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
)
