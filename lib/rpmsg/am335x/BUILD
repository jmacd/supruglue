# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "am335x",
    srcs = [
#	"rpmsg_am335x.c",
    ],
    hdrs = [
#	"rpmsg_am335x.h",
    ],
    deps = [
        "//tools/toolchain/am335x",
#        "@ti-pru-support//include",
        "@ti-pru-support//lib/src/rpmsg_lib",
    ],
)

# cc_library(
#     name = "am335x",
#     srcs = [
#        "@ti-cgt-pru//:libc_a",
#     ],
#     deps = [
#        "@ti-pru-support//include",
#        "@ti-pru-support//lib/src/rpmsg_lib",
#     ],
#     target_compatible_with = select({
#         "//tools/toolchain:am335x": [],
#         "//conditions:default": ["@platforms//:incompatible"],
#     }),
# )
