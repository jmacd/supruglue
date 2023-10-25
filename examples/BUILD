# -*- Mode: Python -*-
cc_binary(
    name = "example_pru0",
    srcs = ["example.c"],
    deps = [
	    "//lib/ui1203",
	    "//lib/cpx",
	    "//lib/rpmsg/am335x",
	    ],
    linkopts = [
        "$(location //tools/toolchain/am335x:pru0)",
    ],
    additional_linker_inputs = [
        "//tools/toolchain/am335x:pru0",
    ],
)

cc_binary(
    name = "local",
    srcs = ["local.c"],
    deps = [
	"//lib/cpx",
	"//supruglue/test32",
	],
)
