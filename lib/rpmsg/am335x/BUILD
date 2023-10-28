# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "rpmsg_am335x.c",
    ],
    hdrs = [
        "rpmsg_am335x.h",
    ],
    deps = [
        "//lib/rpmsg",
        "//supruglue/am335x",
        "//tools/toolchain/am335x",
        "@ti-pru-support//lib/src/rpmsg_lib",
    ],
)
