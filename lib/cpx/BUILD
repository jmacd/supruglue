package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cpx",
    srcs = [
	"cpx.c",
    ],
    hdrs = [
	"cpx.h",
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
        "@gtest//:gtest_main",
        ":cpx",
        ":cpx_test",
    ],
)
