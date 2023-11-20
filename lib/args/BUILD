# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "args",
    srcs = [
        "args.c",
    ],
    hdrs = [
        "args.h",
    ],
)

cc_test(
    name = "test",
    srcs = ["args_test.cc"],
    deps = [
        ":args",
        "@com_google_googletest//:gtest_main",
    ],
)
