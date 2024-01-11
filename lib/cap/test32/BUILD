# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "cap-am335x.cc",
    ],
    hdrs = [
        "cap-am335x.h",
    ],
    deps = [
        "//lib/cap:defs",
    ],
)

cc_test(
    name = "cap_test",
    srcs = ["cap_test.cc"],
    deps = [
        "//lib/cap",
        "@com_google_googletest//:gtest_main",
    ],
)
