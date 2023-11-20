# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "intc-defs.h",
    ],
    deps = [
        "//lib/thread",
    ],
)

cc_library(
    name = "intc",
    hdrs = [
        "intc.h",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/intc/am335x"],
        "//conditions:default": ["//lib/intc/test32"],
    }),
)
