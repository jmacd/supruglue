load("@io_bazel_rules_go//go:def.bzl", "go_library")

go_library(
    name = "remoteproc",
    srcs = ["remoteproc.go"],
    importpath = "github.com/jmacd/supruglue/tools/internal/remoteproc",
    visibility = ["//tools:__subpackages__"],
    deps = ["//tools/internal/firmware"],
)
