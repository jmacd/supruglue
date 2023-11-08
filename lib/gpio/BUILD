# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "gpio-defs.h",
    ],
)

cc_library(
    name = "gpio",
    hdrs = [
        "gpio.h",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/gpio/am335x"],
        "//conditions:default": ["//lib/gpio/test32"],
    }),
)
