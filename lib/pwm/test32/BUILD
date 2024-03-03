# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "pwm-test32.cc",
    ],
    hdrs = [
        "pwm-test32.h",
    ],
    deps = [
        "//lib/pwm:defs",
    ],
)

cc_test(
    name = "test",
    srcs = ["pwm_test.cc"],
    deps = [
        "//lib/pwm",
        "@com_google_googletest//:gtest_main",
    ],
)
