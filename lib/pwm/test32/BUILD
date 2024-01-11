# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "pwm-am335x.cc",
    ],
    hdrs = [
        "pwm-am335x.h",
    ],
    deps = [
        "//lib/pwm:defs",
    ],
)

cc_test(
    name = "pwm_test",
    srcs = ["pwm_test.cc"],
    deps = [
        "//lib/pwm",
        "@com_google_googletest//:gtest_main",
    ],
)
