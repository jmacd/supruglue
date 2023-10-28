# -*- Mode: Python -*-
load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_library")

go_library(
    name = "lib",
    srcs = ["sysevts.go"],
    importpath = "github.com/jmacd/supruglue/tools/cmd/sysevts",
    visibility = ["//visibility:private"],
    deps = [
        "//tools/internal/arch",
        "//tools/internal/csv",
    ],
)

go_binary(
    name = "sysevts",
    embed = [":lib"],
    visibility = ["//visibility:public"],
)
