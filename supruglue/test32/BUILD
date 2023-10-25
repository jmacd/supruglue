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
    ],
    copts = [
        "-Ibazel-out/supruglue/test32",
    ],
    deps = [
	"//supruglue",
        "@ti-pru-support//include",
        "//tools/toolchain/am335x:incompatible",
    ],
)

genrule(
    name = "pinmap_test32_gen",
    srcs = ["//arch:pins"],
    outs = ["pinmap.h"],
    cmd = "./$(location //tools/cmd/pinmap:pinmap) test32 $(location //arch:pins) > $@",
    tools = ["//tools/cmd/pinmap"],
)
