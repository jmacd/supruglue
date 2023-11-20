# -*- Mode: Python -*-
load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_library")

go_library(
    name = "lib",
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
    embed = [":lib"],
    visibility = ["//visibility:public"],
)
