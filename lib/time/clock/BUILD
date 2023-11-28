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
    ] + select({
        "//tools/toolchain:am335x": ["//lib/time/clock/am335x"],
        "//conditions:default": ["//lib/time/clock/test32"],
    }),
)

cc_library(
    name = "process",
    srcs = [
        "process.c",
    ],
    hdrs = [
        "process.h",
    ],
    deps = [
        ":clock",
        "//lib/coroutine",
    ],
)
