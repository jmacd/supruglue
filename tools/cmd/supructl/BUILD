# -*- Mode: Python -*-
load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_library")

go_library(
    name = "lib",
    srcs = ["main.go"],
    importpath = "github.com/jmacd/supruglue/tools/cmd/supructl",
    visibility = ["//visibility:private"],
    deps = [
        "//tools/internal/firmware",
        "//tools/internal/remoteproc",
        "//tools/internal/rpmsghost",
        "@com_github_spf13_cobra//:cobra",
    ],
)

go_binary(
    name = "supructl",
    embed = [":lib"],
    visibility = ["//visibility:public"],
)
