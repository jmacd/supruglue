package(default_visibility = ["//visibility:public"])

cc_library(
    name = "rpmsg_test32",
    srcs = [
	"rpmsg_test32.cc",
    ],
    hdrs = [
	"rpmsg_test32.h",
    ],
    deps = [
        "@com_google_absl//absl/log:initialize",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "rpmsg_test32_test",
    srcs = ["rpmsg_test32_test.cc"],
    deps = [
	":rpmsg_test32",
        "@com_google_googletest//:gtest_main",
    ],
)
