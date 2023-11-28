# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "clock-test32.cc",
    ],
    hdrs = [
        "clock-test32.h",
    ],
    deps = [
        "//lib/thread",
        "//lib/time/clock:defs",
    ],
)

cc_test(
    name = "clock_test",
    srcs = ["clock_test.cc"],
    deps = [
        "//lib/time/clock",
        "//lib/log/fmt",
        "//lib/log/daemon",
        "//lib/rpmsg/test32",
        "@com_google_absl//absl/log",
        "@com_google_googletest//:gtest_main",
    ],
)
