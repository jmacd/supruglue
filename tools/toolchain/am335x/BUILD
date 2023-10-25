# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

filegroup(
    name = "pru0",
    srcs = [
	"pru0.cmd",
    ],
)

filegroup(
    name = "pru1",
    srcs = [
	"pru1.cmd",
    ],
)

cc_library(
    name = "am335x",
    srcs = [
        "@ti-cgt-pru//:libc_a",
    ],
    deps = [
        "@ti-pru-support//include",
    ],
    target_compatible_with = select({
        "//tools/toolchain:am335x": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
)

cc_library(
    name = "incompatible",
    target_compatible_with = select({
        "//tools/toolchain:am335x": ["@platforms//:incompatible"],
        "//conditions:default": [],
    }),
)
