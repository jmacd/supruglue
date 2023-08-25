cc_library(
    name = "supruglue",
    srcs = [],
    hdrs = ["supruglue/gpio.h",
	    "supruglue/soc.h",],
    copts = ["-g"],
)

cc_library(
    name = "test",
    srcs = ["test/soc.cc",
	    "test/gpio.cc"],
    copts = ["-Isupruglue/test32", "-g"],
    hdrs = ["supruglue/test32/include/gpio.h",
	    "supruglue/test32/include/soc.h",],
    deps = [":supruglue",],
    testonly = 1,
)

cc_test(
    name = "gpio_test",
    srcs = ["test/gpio_test.cc"],
    copts = ["-Isupruglue/test32", "-g"],
    deps = [
        "@gtest//:gtest_main",
        ":test",
    ],
)

cc_test(
    name = "soc_test",
    srcs = ["test/soc_test.cc"],
    copts = ["-Isupruglue/test32"],
    deps = [
        "@gtest//:gtest_main",
        ":test",
    ],
)
