# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "period-test32.cc",
    ],
    hdrs = [
        "period-test32.h",
    ],
    deps = [
        "//lib/thread",
        "//lib/time/period:defs",
    ],
)

cc_test(
    name = "period_test",
    srcs = ["period_test.cc"],
    deps = [
        "//lib/time/period",
        # "//lib/log/fmt",
        # "//lib/log/daemon",
        # "//lib/rpmsg/test32",
        # "@com_google_absl//absl/log",
        "@com_google_googletest//:gtest_main",
    ],
)
