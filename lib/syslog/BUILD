# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "syslog",
    srcs = [
        "syslog.c",
    ],
    hdrs = [
        "syslog.h",
    ],
    deps = [
        "//lib/cpx",
        "//lib/rpmsg",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "syslog_test.cc",
    ],
    deps = [
        ":syslog",
        "//lib/cpx:fmt",
        "//lib/rpmsg/test32",
        "@com_google_googletest//:gtest_main",
    ],
)
