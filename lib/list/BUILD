# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

# Note: the generic implementation is currently ~300 bytes larger.
# Re-evaluate. See ./save/list-old.h.
cc_library(
    name = "list",
    hdrs = [
        "list.h",
    ],
    srcs = [
    ],
)

cc_test(
    name = "test",
    srcs = ["list_test.cc"],
    deps = [
        ":list",
        "@com_google_googletest//:gtest_main",
    ],
)
