# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
	"rpmsg_test32_host.cc",
    ],
    hdrs = [
	"rpmsg_test32_host.h",
	"rpmsg_test32_chan.h",
    ],
    deps = [
        "//lib/rpmsg",
        "@com_google_absl//absl/synchronization",
        "//tools/toolchain/am335x:incompatible",
    ],
)

cc_test(
    name = "test",
    srcs = ["rpmsg_test32_test.cc"],
    deps = [
	":test32",
        "@com_google_googletest//:gtest_main",
    ],
)
