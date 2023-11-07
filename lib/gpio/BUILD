# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "gpio",
    hdrs = [
        "gpio.h",
        "gpio-defs.h",
    ],
    deps = select({
        "//tools/toolchain:am335x": ["//lib/gpio/am335x"],
        "//conditions:default": ["//lib/gpio/test32"],
    }),
)
