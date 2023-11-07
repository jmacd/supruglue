# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    hdrs = [
        "gpio.h"
    ],
)

cc_test(
    name = "gpio_test",
    srcs = ["gpio_test.cc"],
    deps = [
        "//supruglue/test32",
        "//lib/gpio",
        "@com_google_googletest//:gtest_main",
    ],
)
