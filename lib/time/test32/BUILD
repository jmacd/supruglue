# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "time.cc",
    ],
    hdrs = [
        "time.h",
    ],
    deps = [
        "//lib/thread",
        "//lib/time:defs",
    ],
)

cc_test(
    name = "time_test",
    srcs = ["time_test.cc"],
    deps = [
        "//lib/time",
        "@com_google_googletest//:gtest_main",
    ],
)
