# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
        "cap-am335x.c",
    ],
    hdrs = [
        "cap-am335x.h",
    ],
    deps = [
        "//lib/cap:defs",
        "//tools/toolchain/am335x",
    ],
)

filegroup(
    name = "pru_ecap_intr_pend",
    srcs = [
        "pru_ecap_intr_pend.yaml",
    ],
)

