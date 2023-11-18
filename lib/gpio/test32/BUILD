# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "gpio.cc",
    ],
    hdrs = [
        "gpio.h",
    ],
    deps = [
        "//lib/gpio:defs",
    ],
)

cc_test(
    name = "gpio_test",
    srcs = ["gpio_test.cc"],
    deps = [
        "//lib/gpio",
        "@com_google_googletest//:gtest_main",
    ],
)
