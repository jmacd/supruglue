# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "rpmsg_test32_host.cc",
    ],
    hdrs = [
        "rpmsg_test32_chan.h",
        "rpmsg_test32_host.h",
    ],
    deps = [
        "//lib/rpmsg",
        "//tools/toolchain/am335x:incompatible",
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
