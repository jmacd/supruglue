# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "gpio.cc",
        "soc.cc",
    ],
    hdrs = [
        "gpio.h",
        "soc.h",
    ],
    copts = [
        "-Ibazel-out/supruglue/test32",
    ],
    deps = [
        "//supruglue",
        "//tools/toolchain/am335x:incompatible",
        "@ti-pru-support//include",
    ],
)
