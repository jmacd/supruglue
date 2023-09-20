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

load("//tools/toolchain/download:local.bzl", "local_archive")

local_archive(
    name = "ti-cgt-pru",
    src = "//tools/toolchain/download:ti_cgt_pru_2.3.3_osx_installer_x86_64.tar.gz",
    build_file = "@//tools/toolchain:ti-cgt-pru.BUILD"
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
    remote = "git@github.com:jmacd/pru-software-support-package.git",
    #branch = "jmacd/6_2_0_BUILD",
    commit = "99c5b892e2e315b8b3bf211c957c26e98c59a870",
)

# Zig CC

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

HERMETIC_CC_TOOLCHAIN_VERSION = "v2.1.0"

http_archive(
    name = "hermetic_cc_toolchain",
    sha256 = "892b0dd7aa88c3504a8821e65c44fd22f32c16afab12d89e9942fff492720b37",
    urls = [
        "https://mirror.bazel.build/github.com/uber/hermetic_cc_toolchain/releases/download/{0}/hermetic_cc_toolchain-{0}.tar.gz".format(HERMETIC_CC_TOOLCHAIN_VERSION),
        "https://github.com/uber/hermetic_cc_toolchain/releases/download/{0}/hermetic_cc_toolchain-{0}.tar.gz".format(HERMETIC_CC_TOOLCHAIN_VERSION),
    ],
)

load("@hermetic_cc_toolchain//toolchain:defs.bzl", zig_toolchains = "toolchains")

# Plain zig_toolchains() will pick reasonable defaults. See
# toolchain/defs.bzl:toolchains on how to change the Zig SDK version and
# download URL.
zig_toolchains()
