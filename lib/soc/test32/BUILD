# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "soc.cc",
    ],
    hdrs = [
        "soc.h",
    ],
    deps = [
        "//lib/soc:defs",
        "@com_google_absl//absl/synchronization",
        "//tools/toolchain/am335x:incompatible",
        "@ti-pru-support//include",
    ],
)
