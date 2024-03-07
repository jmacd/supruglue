# -*- Mode: Python -*-
load("@io_bazel_rules_go//go:def.bzl", "go_library")

go_library(
    name = "arch",
    srcs = [
        "irq.go",
        "module.go",
        "pin.go",
        "register.go",
        "sysevt.go",
    ],
    importpath = "github.com/jmacd/supruglue/tools/internal/arch",
    visibility = ["//tools:__subpackages__"],
)
