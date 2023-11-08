# -*- Mode: Python -*-

cc_library(
    name = "supruglue",
    hdrs = [
        "soc.h",
        "supruglue.h",
        ":sysevts_gen",
    ],
    visibility = ["//visibility:public"],
)

genrule(
    name = "sysevts_gen",
    srcs = ["//arch:sysevts"],
    outs = ["sysevts-arch.h"],
    cmd = "./$(location //tools/cmd/sysevts:sysevts) $(location //arch:sysevts) > $@",
    tools = ["//tools/cmd/sysevts"],
)
