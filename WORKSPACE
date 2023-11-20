# Common
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Googletest C++ test library

git_repository(
    name = "com_google_googletest",
    branch = "v1.14.x",
    remote = "https://github.com/google/googletest",
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
# Use `bazel run //:gazelle update-repos -- -from_file=go.mod`
# to update dependcies
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
load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies", "go_repository")

# gazelle:prefix github.com/jmacd/supruglue

go_repository(
    name = "com_github_mitchellh_mapstructure",
    importpath = "github.com/mitchellh/mapstructure",
    sum = "h1:jeMsZIYE/09sWLaz43PL7Gy6RuMjD2eJVyuac5Z2hdY=",
    version = "v1.5.0",
)

go_repository(
    name = "in_gopkg_check_v1",
    importpath = "gopkg.in/check.v1",
    sum = "h1:yhCVgyC4o1eVCa2tZl7eS0r+SDo693bJlVdllGtEeKM=",
    version = "v0.0.0-20161208181325-20d25e280405",
)

go_repository(
    name = "in_gopkg_yaml_v3",
    importpath = "gopkg.in/yaml.v3",
    sum = "h1:fxVm/GzAzEWqLHuvctI91KS9hhNmmWOoWu0XTYJS7CA=",
    version = "v3.0.1",
)

go_repository(
    name = "com_github_cpuguy83_go_md2man_v2",
    importpath = "github.com/cpuguy83/go-md2man/v2",
    sum = "h1:qMCsGGgs+MAzDFyp9LpAe1Lqy/fY/qCovCm0qnXZOBM=",
    version = "v2.0.3",
)

go_repository(
    name = "com_github_inconshreveable_mousetrap",
    importpath = "github.com/inconshreveable/mousetrap",
    sum = "h1:wN+x4NVGpMsO7ErUn/mUI3vEoE6Jt13X2s0bqwp9tc8=",
    version = "v1.1.0",
)

go_repository(
    name = "com_github_russross_blackfriday_v2",
    importpath = "github.com/russross/blackfriday/v2",
    sum = "h1:JIOH55/0cWyOuilr9/qlrm0BSXldqnqwMsf35Ld67mk=",
    version = "v2.1.0",
)

go_repository(
    name = "com_github_spf13_cobra",
    importpath = "github.com/spf13/cobra",
    sum = "h1:7aJaZx1B85qltLMc546zn58BxxfZdR/W22ej9CFoEf0=",
    version = "v1.8.0",
)

go_repository(
    name = "com_github_spf13_pflag",
    importpath = "github.com/spf13/pflag",
    sum = "h1:iy+VFUOCP1a+8yFto/drg2CJ5u0yRoB7fZw3DKv/JXA=",
    version = "v1.0.5",
)

gazelle_dependencies()

# see https://stackoverflow.com/questions/58937356/bazel-relative-local-path-as-url-in-http-archive
#http_archive(
#    name = "ti-cgt-pru",
#    url = "file://tools/download/ti_cgt_pru_2.3.3_linux_installer_x86.tar.gz",
#    sha256 = "b5fe14f178e4b074ddfbdc711a59dbe5bf80e9d95c5a366d20ef6f587a1af70e",
#    build_file = "@//tools/download/ti-cgt-pru.BUILD.bazel"
#)

load("//tools/toolchain/download:local.bzl", "local_archive")

local_archive(
    name = "ti-cgt-pru",
    src = "//tools/toolchain/download:ti_cgt_pru_2.3.3_osx_installer_x86_64.tar.gz",
    build_file = "@//tools/toolchain:ti-cgt-pru.BUILD",
)

# PRU software support
# git_repository(
#     name = "ti-pru-support",
#     remote = "git://git.ti.com/pru-software-support-package/pru-software-support-package.git",
#     tag = "v6.2.0",
#     build_file = "@//tools/toolchain:ti-pru-support.BUILD"
# )

# This fork of the above has a few workarounds.
git_repository(
    name = "ti-pru-support",
    #branch = "jmacd/6_2_0_BUILD",
    commit = "99c5b892e2e315b8b3bf211c957c26e98c59a870",
    remote = "git@github.com:jmacd/pru-software-support-package.git",
)

# Abseil

http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-99bbd7c4865c5d0f3bfbe99457c5ce4daf6ba174",
    urls = ["https://github.com/abseil/abseil-cpp/archive/99bbd7c4865c5d0f3bfbe99457c5ce4daf6ba174.zip"],
)

http_archive(
    name = "com_github_google_benchmark",
    sha256 = "2a778d821997df7d8646c9c59b8edb9a573a6e04c534c01892a40aa524a7b68c",
    strip_prefix = "benchmark-bf585a2789e30585b4e3ce6baf11ef2750b54677",
    urls = ["https://github.com/google/benchmark/archive/bf585a2789e30585b4e3ce6baf11ef2750b54677.zip"],
)

http_archive(
    name = "bazel_skylib",
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
    urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
)
