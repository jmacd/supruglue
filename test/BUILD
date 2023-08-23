cc_library(
    name = "test",
    hdrs = [
	 "gpio.h",
	 "soc.h"
    ],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
        "//:gpio",
    ],
)

cc_test(
    name = "gpio_test",
    srcs = ["gpio_test.cc"],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
        ":test",
        "//:gpio",
    ],
)
