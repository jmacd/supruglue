load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_library")

go_library(
    name = "pinmap_lib",
    srcs = ["pinmap.go"],
    importpath = "github.com/jmacd/supruglue/tools/cmd/pinmap",
    visibility = ["//visibility:private"],
    deps = [
        "//tools/internal/arch",
        "//tools/internal/csv",
	],
)

go_binary(
    name = "pinmap",
    embed = [":pinmap_lib"],
    visibility = ["//visibility:public"],
)
