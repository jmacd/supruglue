# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "rpmsg.cc",
    ],
    hdrs = [
        "chan.h",
        "rpmsg.h",
    ],
    deps = [
        "//lib/rpmsg:defs",
        "//lib/coroutine",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "test",
    srcs = ["rpmsg_test.cc"],
    deps = [
        ":test32",
        "@com_google_googletest//:gtest_main",
    ],
)
