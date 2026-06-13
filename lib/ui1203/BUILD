# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ui1203",
    srcs = [
        "ui1203.c",
    ],
    hdrs = [
        "ui1203.h",
    ],
    deps = [
        "//lib/gpio",
        "//lib/coroutine",
        "//lib/log/journal",
        "//lib/rpmsg",
        "//lib/time",
        ],
)

cc_test(
    name = "test",
    srcs = ["ui1203_test.cc"],
    deps = [
        ":ui1203",
        "//lib/coroutine",
        "//lib/gpio",
        "//lib/gpio/test32",
        "//lib/intc",
        "//lib/intc:service",
        "//lib/pinmap",
        "//lib/rpmsg/test32",
        "//lib/time:process",
        "@com_google_googletest//:gtest_main",
    ],
)
