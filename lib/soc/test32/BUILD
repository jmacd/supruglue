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
        "//tools/toolchain/am335x:incompatible",
        "@com_google_absl//absl/synchronization",
        "@ti-pru-support//include",
    ],
)
