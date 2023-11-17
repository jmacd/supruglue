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
        "//lib/rpmsg",
        "//lib/intc",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "daemon_test.cc",
    ],
    deps = [
        ":daemon",
        "//lib/log/fmt",
        "//lib/intc",
        "//lib/rpmsg/test32",
        "@com_google_absl//absl/log",
        "@com_google_googletest//:gtest_main",
    ],
)
