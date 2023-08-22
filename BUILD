cc_library(
    name = "gpio",
    srcs = ["gpio.cc"],
    hdrs = ["include/gpio.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["src/gpio.cc"],
    deps = [
        ":hello-greet",
    ],
)
