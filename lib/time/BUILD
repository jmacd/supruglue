# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "time-defs.h",
    ],
    deps = [
        "//lib/soc",
    ],
)

cc_library(
    name = "time",
    hdrs = [
        "time.h",
    ],
    srcs = [
	"time.c",
    ],
    deps = [
        ":defs",
        "//lib/coroutine",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/time/am335x"],
        "//conditions:default": ["//lib/time/test32"],
    }),
)