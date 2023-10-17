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
        "//lib/rpmsg/am335x:iface",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/synchronization",
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
