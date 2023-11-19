load("@io_bazel_rules_go//go:def.bzl", "go_library")

go_library(
    name = "firmware",
    srcs = ["firmware.go"],
    importpath = "github.com/jmacd/supruglue/tools/internal/firmware",
    visibility = ["//tools:__subpackages__"],
    deps = ["//tools/internal/elfdata"],
)
