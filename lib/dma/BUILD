# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "dma-defs.h",
    ],
    deps = [
        "//lib/pinmap",
    ],
)

cc_library(
    name = "dma",
    hdrs = [
        "dma.h",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/dma/am335x"],
        "//conditions:default": ["//lib/dma/test32"],
    }),
)
