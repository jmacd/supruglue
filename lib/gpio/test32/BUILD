# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    hdrs = [
        "gpio.h"
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
        "//lib/soc",
        "@com_google_googletest//:gtest_main",
    ],
)
