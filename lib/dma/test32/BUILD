# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test32",
    srcs = [
        "dma-test32.cc",
    ],
    hdrs = [
        "dma-test32.h",
    ],
    deps = [
        "//lib/dma:defs",
    ],
)

cc_test(
    name = "test",
    srcs = ["dma_test.cc"],
    deps = [
        "//lib/dma",
        "@com_google_googletest//:gtest_main",
    ],
)
