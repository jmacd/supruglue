# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    hdrs = [
        "time.h"
    ],
    srcs = [
        "time.cc",
    ],
    deps = [
            "//lib/time:defs",
	    "//lib/thread",
    ],
)

cc_test(
    name = "time_test",
    srcs = ["time_test.cc"],
    deps = [
        "//lib/time",
        "@com_google_googletest//:gtest_main",
    ],
)
