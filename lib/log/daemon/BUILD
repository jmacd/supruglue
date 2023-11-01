# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "daemon",
    srcs = [
        "daemon.c",
    ],
    hdrs = [
        "daemon.h",
    ],
    deps = [
        "//lib/cpx",
        "//lib/rpmsg",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "daemon_test.cc",
    ],
    deps = [
        ":daemon",
        "//lib/cpx:fmt",
        "//lib/rpmsg/test32",
        "@com_google_googletest//:gtest_main",
    ],
)
