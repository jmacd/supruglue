# -*- Mode: Python -*-

cc_library(
    name = "supruglue",
    hdrs = [
        "pinmap.h",
        "soc.h",
        "supruglue.h",
        ":pinmap_gen",
        ":sysevts_gen",
    ],
    visibility = ["//visibility:public"],
)

genrule(
    name = "pinmap_gen",
    srcs = ["//arch:pins"],
    outs = ["pinmap-arch.h"],
    cmd = "./$(location //tools/cmd/pinmap:pinmap) $(location //arch:pins) > $@",
    tools = ["//tools/cmd/pinmap"],
)

genrule(
    name = "sysevts_gen",
    srcs = ["//arch:sysevts"],
    outs = ["sysevts-arch.h"],
    cmd = "./$(location //tools/cmd/sysevts:sysevts) $(location //arch:sysevts) > $@",
    tools = ["//tools/cmd/sysevts"],
)
