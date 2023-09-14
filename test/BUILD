cc_library(
    name = "test",
    testonly = 1,
    srcs = [
        "gpio.cc",
        "soc.cc",
    ],
    deps = [
	"//supruglue",
        "//supruglue/test32",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "gpio_test",
    srcs = ["gpio_test.cc"],
    copts = [],
    deps = [
        ":test",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "soc_test",
    srcs = ["soc_test.cc"],
    copts = ["-Isupruglue/test32"],
    deps = [
        ":test",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "pinmap_test",
    srcs = ["pinmap_test.cc"],
    copts = ["-Isupruglue/test32"],
    deps = [
        ":test",
        "@gtest//:gtest_main",
    ],
)
