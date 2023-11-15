# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "defs",
    hdrs = [
        "soc-defs.h",
    ],
    deps = [
    ],
)

cc_library(
    name = "soc",
    hdrs = [
        "soc.h",
        ":sysevts_gen",
    ],
    deps = [
        ":defs",
    ] + select({
        "//tools/toolchain:am335x": ["//lib/soc/am335x"],
        "//conditions:default": ["//lib/soc/test32"],
    }),
)

genrule(
    name = "sysevts_gen",
    srcs = ["//arch:sysevts"],
    outs = ["sysevts.h"],
    cmd = "./$(location //tools/cmd/sysevts:sysevts) $(location //arch:sysevts) > $@",
    tools = ["//tools/cmd/sysevts"],
)

cc_test(
    name = "test",
    srcs = ["soc_test.cc"],
    deps = [
        ":soc",
        "@com_google_googletest//:gtest_main",
    ],
)
