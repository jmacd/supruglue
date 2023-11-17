# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "coroutine",
    srcs = [
        "coroutine.c",
    ],
    hdrs = [
        "coroutine.h",
    ],
    deps = [
        "//lib/thread",
        "//lib/intc",
        "//lib/log/journal",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "coroutine_test.cc",
    ],
    deps = [
        ":coroutine",
        "//lib/log/fmt",
        "@com_google_googletest//:gtest_main",
    ],
)
