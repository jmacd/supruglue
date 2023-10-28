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
        ":pinmap_test32_gen",
        ":sysevts_test32_gen",
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

genrule(
    name = "pinmap_test32_gen",
    srcs = ["//arch:pins"],
    outs = ["pinmap.h"],
    cmd = "./$(location //tools/cmd/pinmap:pinmap) test32 $(location //arch:pins) > $@",
    tools = ["//tools/cmd/pinmap"],
)

genrule(
    name = "sysevts_test32_gen",
    srcs = ["//arch:sysevts"],
    outs = ["sysevts.h"],
    cmd = "./$(location //tools/cmd/sysevts:sysevts) $(location //arch:sysevts) > $@",
    tools = ["//tools/cmd/sysevts"],
)
