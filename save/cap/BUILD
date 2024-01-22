# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "cap-defs.h",
    ],
    deps = [
        "//lib/pinmap",
    ],
)

cc_library(
    name = "cap",
    hdrs = [
        "cap.h",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/cap/am335x"],
        "//conditions:default": ["//lib/cap/test32"],
    }),
)
