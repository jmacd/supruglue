# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ui1203",
    srcs = [
        "ui1203.c",
    ],
    hdrs = [
        "ui1203.h",
    ],
    deps = ["//lib/gpio"],
)

cc_test(
    name = "test",
    srcs = ["ui1203_test.cc"],
    deps = [
        ":ui1203",
        "@com_google_googletest//:gtest_main",
    ],
)
