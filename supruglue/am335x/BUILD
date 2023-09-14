cc_library(
    name = "am335x",
    srcs = [
    ],
    hdrs = [
        "gpio.h",
        "soc.h",
        ":pinmap_am335x_gen",
    ],
    copts = [
    ],
    deps = ["//supruglue"],
    visibility = ["//visibility:public"],
)

genrule(
    name = "pinmap_am335x_gen",
    srcs = ["//arch"],
    outs = ["pinmap.h"],
    cmd = "./$(location //tools/cmd/pinmap:pinmap) am335x $(location //arch) > $@",
    tools = ["//tools/cmd/pinmap"],
)

