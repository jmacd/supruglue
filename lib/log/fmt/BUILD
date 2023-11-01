# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fmt",
    srcs = [
        "fmt.c",
    ],
    hdrs = [
        "fmt.h",
	],
    testonly = True,
    deps = [
	"//lib/cpx",
	"//tools/toolchain/am335x:incompatible",
        "@com_google_absl//absl/strings:str_format",
    ],
)
