# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cpx",
    srcs = [
        "args.c",
        "cpx.c",
    ],
    hdrs = [
        "args.h",
        "cpx.h",
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
        "//tools/toolchain/am335x:incompatible",
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "args_test.cc",
        "cpx_test.cc",
    ],
    deps = [
        ":cpx",
        ":fmt",
        "//tools/toolchain/am335x:incompatible",
        "@com_google_googletest//:gtest_main",
    ],
)
