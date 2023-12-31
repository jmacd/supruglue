# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "initproc",
    srcs = [
        "initproc.c",
    ],
    hdrs = [
        "initproc.h",
    ],
    deps = [
        "//lib/coroutine",
        "//lib/debug",
        "//lib/intc",
        "//lib/log/journal",
        "//lib/rpmsg",
        "//lib/time",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "initproc_test.cc",
    ],
    deps = [
        ":initproc",
        "//lib/intc",
        "//lib/intc:service",
        "//lib/log/daemon",
        "//lib/log/fmt",
        "//lib/rpmsg/test32",
        "//lib/time:process",
        "@com_google_absl//absl/log",
        "@com_google_googletest//:gtest_main",
    ],
)
