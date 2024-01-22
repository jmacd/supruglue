# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    hdrs = [
        "pwm-am335x.h",
    ],
    srcs = [
        "pwm-am335x.c",
    ],
    deps = [
        "//lib/pwm:defs",
        "//tools/toolchain/am335x",
    ],
)

filegroup(
    name = "epwm0_intr_pend",
    srcs = [
        "epwm0_intr_pend.yaml",
    ],
)
