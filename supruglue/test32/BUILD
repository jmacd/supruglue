cc_library(
    name = "test32",
    srcs = [
        "gpio.cc",
        "soc.cc",
    ],
    hdrs = [
	"gpio.h",
	"soc.h",
        ":pinmap_test32_gen",
    ],
    copts = [
        "-Ibazel-out/supruglue/test32",
    ],
    deps = [
	"//supruglue",
        "@ti-pru-support//include",

        # Note: we can't build this w/o clpru
        # so we replace it
        # "@ti-pru-support//lib/src/rpmsg_lib",
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
