# -*- Mode: Python -*-
load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_library")

go_library(
    name = "lib",
    srcs = ["irqgen.go"],
    importpath = "github.com/jmacd/supruglue/tools/cmd/irqgen",
    visibility = ["//visibility:private"],
    deps = [
        "//tools/internal/arch",
        "//tools/internal/csv",
        "@com_github_mitchellh_mapstructure//:mapstructure",
        "@in_gopkg_yaml_v3//:yaml_v3",
    ],
)

go_binary(
    name = "irqgen",
    embed = [":lib"],
    visibility = ["//visibility:public"],
)
