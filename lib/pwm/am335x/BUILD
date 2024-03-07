# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "pwm-am335x.c",
    ],
    hdrs = [
        "pwm-am335x.h",
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

filegroup(
    name = "epwm1_intr_pend",
    srcs = [
        "epwm1_intr_pend.yaml",
    ],
)

filegroup(
    name = "epwm2_intr_pend",
    srcs = [
        "epwm2_intr_pend.yaml",
    ],
)
