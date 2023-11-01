# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "thread",
    srcs = [
        "thread.c",
    ],
    hdrs = [
        "thread.h",
    ],
    deps = [
	"//lib/args",
	"//lib/list",
    ],
)

cc_test(
    name = "test",
    srcs = ["thread_test.cc"],
    deps = [
        ":thread",
        "@com_google_googletest//:gtest_main",
    ],
)
