# Common
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Googletest C++ test library

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    branch = "v1.14.x",
)

# Go setup

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "51dc53293afe317d2696d4d6433a4c33feedb7748a9e352072e2ec3c0dafd2c6",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.40.1/rules_go-v0.40.1.zip",
        "https://github.com/bazelbuild/rules_go/releases/download/v0.40.1/rules_go-v0.40.1.zip",
    ],
)

# Gazelle
http_archive(
    name = "bazel_gazelle",
    sha256 = "727f3e4edd96ea20c29e8c2ca9e8d2af724d8c7778e7923a854b2c80952bc405",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-gazelle/releases/download/v0.30.0/bazel-gazelle-v0.30.0.tar.gz",
        "https://github.com/bazelbuild/bazel-gazelle/releases/download/v0.30.0/bazel-gazelle-v0.30.0.tar.gz",
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")

go_rules_dependencies()

go_register_toolchains(version = "1.20.7")

load("@bazel_gazelle//:def.bzl", "gazelle")
load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies")

# gazelle:prefix github.com/jmacd/supruglue

gazelle_dependencies()

# see https://stackoverflow.com/questions/58937356/bazel-relative-local-path-as-url-in-http-archive
#http_archive(
#    name = "ti-cgt-pru",
#    url = "file://tools/download/ti_cgt_pru_2.3.3_linux_installer_x86.tar.gz",
#    sha256 = "b5fe14f178e4b074ddfbdc711a59dbe5bf80e9d95c5a366d20ef6f587a1af70e",
#    build_file = "@//tools/download/ti-cgt-pru.BUILD.bazel"
#)

load("//tools/download:local.bzl", "local_archive")

local_archive(
    name = "ti-cgt-pru",
    src = "//tools/download:ti_cgt_pru_2.3.3_linux_installer_x86.tar.gz",
    build_file = "@//tools/download:ti-cgt-pru.BUILD.bazel"
)
