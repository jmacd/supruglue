# -*- Mode: Python -*-
package(default_visibility = ["//visibility:public"])

load("@io_bazel_rules_go//go:def.bzl", "go_library")

filegroup(
    name = "defs",
    srcs = glob(["*.csv"]),
)

go_library(
    name = "am335x",
    srcs = ["embed.go"],
    embedsrcs = [
        ":defs",
    ],
    importpath = "github.com/jmacd/supruglue/arch/am335x",
)
