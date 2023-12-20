# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "clock-defs.h",
    ],
    deps = [
        "//lib/soc",
        "//lib/sync",
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

cc_test(
    name = "test",
    srcs = ["clock_test.cc"],
    deps = [
        "//lib/time/clock:process",
        "//lib/log/fmt",
        "//lib/log/daemon",
        "//lib/rpmsg/test32",
        "@com_google_absl//absl/log",
        "@com_google_googletest//:gtest_main",
    ],
)
