# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cpx",
    srcs = [
        "cpx.c",
    ],
    hdrs = [
        "cpx.h",
    ],
    deps = [
        "//lib/thread",
        "//lib/log/journal",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "cpx_test.cc",
    ],
    deps = [
        ":cpx",
        "//lib/log/fmt",
        "@com_google_googletest//:gtest_main",
    ],
)
