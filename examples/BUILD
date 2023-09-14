
cc_binary(
    name = "example",
    srcs = ["example.c"],
    deps = [
	    "//lib/ui1203",
	    "//lib/cpx",
	    "//tools/toolchain/am335x:runtime",
	    ],
    linkopts = [
        "$(location //tools/toolchain/am335x:cmd)",
    ],
    additional_linker_inputs = [
        "//tools/toolchain/am335x:cmd",
    ],
)
