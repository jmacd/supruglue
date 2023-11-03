# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "journal",
    srcs = [
        "journal.c",
    ],
    hdrs = [
        "journal.h",
	],
    deps = [
	"//lib/list",
	"//lib/thread",
	"//lib/sync",
    ],
)

cc_test(
    name = "test",
    srcs = ["journal_test.cc"],
    deps = [
        ":journal",
        "@com_google_googletest//:gtest_main",
    ],
)
