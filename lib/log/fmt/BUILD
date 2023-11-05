# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fmt",
    srcs = [
        "fmt.cc",
    ],
    hdrs = [
        "fmt.h",
	],
    testonly = True,
    deps = [
	"//lib/coroutine",
	"//tools/toolchain/am335x:incompatible",
        "@com_google_absl//absl/strings:str_format",
    ],
)
