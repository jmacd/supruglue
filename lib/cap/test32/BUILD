# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "cap-test32.cc",
    ],
    hdrs = [
        "cap-test32.h",
    ],
    deps = [
        "//lib/cap:defs",
    ],
)

cc_test(
    name = "test",
    srcs = ["cap_test.cc"],
    deps = [
        "//lib/cap",
        "@com_google_googletest//:gtest_main",
    ],
)
