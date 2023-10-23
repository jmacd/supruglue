# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cpx",
    srcs = [
	"cpx.c",
	"args.c",
    ],
    hdrs = [
	"cpx.h",
	"args.h",
    ],
)

cc_library(
    name = "fmt",
    srcs = [
	"fmt.cc",
    ],
    hdrs = [
	"fmt.h",
    ],
    deps = [
	":cpx",
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_library(
    name = "cpx_test",
    srcs = [
	"cpx_test_c.c",
    ],
    hdrs = [
	"cpx_test_c.h",
    ],
    deps = [
	":cpx",
    ],
    testonly = 1,
)

cc_test(
    name = "test",
    srcs = [
        "cpx_test.cc",
    ],
    deps = [
        "@com_google_googletest//:gtest_main",
        ":cpx",
        ":fmt",
        ":cpx_test",
    ],
)
