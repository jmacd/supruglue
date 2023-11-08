# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "pinmap",
    hdrs = [
        "pinmap-defs.h",
        ":pinmap_gen",
    ],
    deps = [
        "//lib/soc",
    ]
)

genrule(
    name = "pinmap_gen",
    srcs = ["//arch:pins"],
    outs = ["pinmap.h"],
    cmd = "./$(location //tools/cmd/pinmap:pinmap) $(location //arch:pins) > $@",
    tools = ["//tools/cmd/pinmap"],
)

cc_test(
    name = "test",
    srcs = ["pinmap_test.cc"],
    deps = [
        ":pinmap",
        "@com_google_googletest//:gtest_main",
    ],
)
