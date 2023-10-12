
cc_binary(
    name = "example_pru",
    srcs = ["example.c"],
    deps = [
	    "//lib/ui1203",
	    "//lib/cpx",
	    "//tools/toolchain/am335x",
	    ],
    linkopts = [
        "$(location //tools/toolchain/am335x:cmd)",
    ],
    additional_linker_inputs = [
        "//tools/toolchain/am335x:cmd",
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
