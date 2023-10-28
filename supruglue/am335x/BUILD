# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "soc.c",
    ],
    hdrs = [
        "gpio.h",
        "soc.h",
        ":pinmap_am335x_gen",
        ":sysevts_am335x_gen",
    ],
    copts = [
    ],
    deps = ["//supruglue"],
)

genrule(
    name = "pinmap_am335x_gen",
    srcs = ["//arch:pins"],
    outs = ["pinmap.h"],
    cmd = "./$(location //tools/cmd/pinmap:pinmap) am335x $(location //arch:pins) > $@",
    tools = ["//tools/cmd/pinmap"],
)

genrule(
    name = "sysevts_am335x_gen",
    srcs = ["//arch:sysevts"],
    outs = ["sysevts.h"],
    cmd = "./$(location //tools/cmd/sysevts:sysevts) $(location //arch:sysevts) > $@",
    tools = ["//tools/cmd/sysevts"],
)
