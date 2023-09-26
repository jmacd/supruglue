package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cpx",
    srcs = [
	"CorePartition.c",
    ],
    hdrs = [
	"CorePartition.h",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "cpx_test.cc",
    ],
    deps = [
        "@gtest//:gtest_main",
        ":cpx",
    ],
)
