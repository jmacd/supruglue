# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "daemon",
    srcs = [
        "daemon.c",
    ],
    hdrs = [
        "daemon.h",
    ],
    deps = [
        "//lib/coroutine",
        "//lib/intc",
        "//lib/rpmsg",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "daemon_test.cc",
    ],
    deps = [
        ":daemon",
        "//lib/intc",
        "//lib/intc:service",
        "//lib/log/fmt",
        "//lib/rpmsg/test32",
        "//lib/time:process",
        "@com_google_absl//absl/log",
        "@com_google_googletest//:gtest_main",
    ],
)
