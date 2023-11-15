# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "rpmsg-defs.h",
    ],
)

cc_library(
    name = "rpmsg",
    hdrs = [
        "rpmsg.h",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/rpmsg/am335x"],
        "//conditions:default": ["//lib/rpmsg/test32"],
    }),
)
