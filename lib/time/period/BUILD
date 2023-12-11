# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

filegroup(
    name = "epwm0_01",
    srcs = [
        "epwm0_01.yaml",
    ],
)

cc_library(
    name = "defs",
    hdrs = [
        "period-defs.h",
    ],
    deps = [
        "//lib/soc",
    ],
)

cc_library(
    name = "period",
    srcs = [
        "period.c",
    ],
    hdrs = [
        "period.h",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/time/period/am335x"],
        "//conditions:default": ["//lib/time/period/test32"],
    }),
)
