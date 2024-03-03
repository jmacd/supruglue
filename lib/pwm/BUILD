# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "pwm-defs.h",
    ],
    deps = [
        "//lib/pinmap",
    ],
)

cc_library(
    name = "pwm",
    hdrs = [
        "pwm.h",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/pwm/am335x"],
        "//conditions:default": ["//lib/pwm/test32"],
    }),
)
