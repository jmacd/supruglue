# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "list",
    hdrs = [
        "list.h",
        "generic.h",
    ],
    srcs = [
        "generic.c",
    ],
)

cc_test(
    name = "test",
    srcs = ["list_test.cc"],
    deps = [
        ":list",
        "@com_google_googletest//:gtest_main",
    ],
)
