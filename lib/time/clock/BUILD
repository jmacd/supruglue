# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "clock-defs.h",
    ],
    deps = [
        "//lib/soc",
    ],
)

cc_library(
    name = "clock",
    srcs = [
        "clock.c",
    ],
    hdrs = [
        "clock.h",
    ],
    deps = [
        ":defs",
        "//lib/coroutine",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/time/clock/am335x"],
        "//conditions:default": ["//lib/time/clock/test32"],
    }),
)
