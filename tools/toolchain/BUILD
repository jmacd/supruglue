# hmm https://groups.google.com/g/bazel-discuss/c/fsA54jZbSH0
# example
# https://github.com/frc971/971-Robot-Code/blob/master/tools/cpp/toolchain_config.bzl#L1599

package(default_visibility = ["//visibility:public"])

cc_toolchain_suite(
    name = "clpru_suite",
    toolchains = {
        "pru": ":pru_toolchain",
        "darwin": ":pru_toolchain",
    },
)

filegroup(
    name = "wrappers_all",
    srcs = glob([
        "wrappers/*",
    ]) + [
        "@ti-cgt-pru//:all_files",
    ],
)

cc_toolchain(
    name = "pru_toolchain",
    all_files = ":wrappers_all",
    compiler_files = ":wrappers_all",
    dwp_files = ":wrappers_all",
    linker_files = ":wrappers_all",
    objcopy_files = ":wrappers_all",
    strip_files = ":wrappers_all",
    supports_param_files = 0,
    toolchain_config = ":pru_toolchain_config",
    toolchain_identifier = "pru-toolchain",
)

load(":cc_toolchain_config.bzl", "cc_toolchain_config")

cc_toolchain_config(
    name = "pru_toolchain_config",
)

constraint_setting(name = "pru_soc")

constraint_value(
    name = "am335x",
    constraint_setting = ":pru_soc",
)

constraint_value(
    name = "test32",
    constraint_setting = ":pru_soc",
)

constraint_setting(name = "pru_num")

constraint_value(
    name = "pru0",
    constraint_setting = ":pru_num",
)

constraint_value(
    name = "pru1",
    constraint_setting = ":pru_num",
)

# See https://github.com/bazelbuild/rules_go/issues/2591
# why for Go toolchain selection we list host platform os/cpu.
platform(
    name = "real_pru0",
    constraint_values = [
        ":am335x",
        ":pru0",
        "@platforms//os:osx",
        "@platforms//cpu:arm64",
        "@io_bazel_rules_go//go/toolchain:cgo_off",
    ],
)

platform(
    name = "real_pru1",
    constraint_values = [
        ":am335x",
        ":pru1",
        "@platforms//os:osx",
        "@platforms//cpu:arm64",
        "@io_bazel_rules_go//go/toolchain:cgo_off",
    ],
)
