cc_library(
    name = "test32",
    hdrs = [
	"gpio.h",
	"soc.h",
        ":pinmap_test32_gen",
    ],
    copts = [
        "-Ibazel-out/supruglue/test32",
    ],
    deps = [
        "@ti-pru-support//include",
        "@ti-pru-support//lib/src/rpmsg_lib",
    ],
    visibility = ["//visibility:public"],
    target_compatible_with = select({
        "//tools/toolchain:am335x": ["@platforms//:incompatible"],
        "//conditions:default": [],
    }),
)

genrule(
    name = "pinmap_test32_gen",
    srcs = ["//arch"],
    outs = ["pinmap.h"],
    cmd = "./$(location //tools/cmd/pinmap:pinmap) test32 $(location //arch) > $@",
    tools = ["//tools/cmd/pinmap"],
)
