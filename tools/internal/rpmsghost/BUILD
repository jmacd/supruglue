load("@io_bazel_rules_go//go:def.bzl", "go_library")

go_library(
    name = "rpmsghost",
    srcs = ["rpmsghost.go"],
    importpath = "github.com/jmacd/supruglue/tools/internal/rpmsghost",
    visibility = ["//tools:__subpackages__"],
    deps = [
        "//tools/internal/firmware",
        "//tools/internal/fwstate",
        "//tools/internal/remoteproc",
    ],
)
