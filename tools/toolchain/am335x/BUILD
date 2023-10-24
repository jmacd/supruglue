# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

filegroup(
    name = "cmd",
    srcs = [
	"AM335x_PRU_intc_rscTbl.cmd",
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
