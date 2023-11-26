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
        "@com_google_googletest//:gtest_main",
    ],
)
