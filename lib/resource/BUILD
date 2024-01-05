# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "resource",
    srcs = [
        "table.c",
    ],
    hdrs = [
        "table.h",
    ],
    deps = [
        "@ti-pru-support//include",
    ],
    target_compatible_with = select({
        "//tools/toolchain:am335x": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
)
