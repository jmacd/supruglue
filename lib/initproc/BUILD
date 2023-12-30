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
        "//lib/debug",
        "//lib/intc",
        "//lib/rpmsg",
        "//lib/time",
        "//lib/coroutine",
        "//lib/log/journal",
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
        "//lib/log/fmt",
        "//lib/rpmsg/test32",
        "//lib/time:process",
        "//lib/log/daemon",
        "//lib/intc:service",
        "@com_google_absl//absl/log",
        "@com_google_googletest//:gtest_main",
    ],
)
