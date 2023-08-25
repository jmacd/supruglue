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
    copts = ["-Isupruglue/test", "-g"],
    hdrs = ["supruglue/test/include/gpio.h",
	    "supruglue/test/include/soc.h",],
    deps = [":supruglue",],
    testonly = 1,
)

cc_test(
    name = "gpio_test",
    srcs = ["test/gpio_test.cc"],
    copts = ["-Isupruglue/test", "-g"],
    deps = [
        "@gtest//:gtest_main",
        ":test",
    ],
)

cc_test(
    name = "soc_test",
    srcs = ["test/soc_test.cc"],
    copts = ["-Isupruglue/test"],
    deps = [
        "@gtest//:gtest_main",
        ":test",
    ],
)
