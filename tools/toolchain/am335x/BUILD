# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

filegroup(
    name = "pru0_linker_cmd",
    srcs = [
        "pru0.cmd",
    ],
)

filegroup(
    name = "pru1_linker_cmd",
    srcs = [
        "pru1.cmd",
    ],
)

filegroup(
    name = "linker_cmd",
    srcs = select({
        "//tools/toolchain:pru0": [
            "pru0.cmd",
        ],
        "//tools/toolchain:pru1": [
            "pru1.cmd",
        ],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
)

cc_library(
    name = "am335x",
    srcs = [
        "@ti-cgt-pru//:libc_a",
    ],
    target_compatible_with = select({
        "//tools/toolchain:am335x": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
    deps = [
        "@ti-pru-support//include",
        "@ti-pru-support//include/am335x",
    ],
)

cc_library(
    name = "incompatible",
    target_compatible_with = select({
        "//tools/toolchain:am335x": ["@platforms//:incompatible"],
        "//conditions:default": [],
    }),
)
