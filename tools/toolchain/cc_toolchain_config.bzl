load("@rules_cc//cc:action_names.bzl", "ACTION_NAMES")
load(
    "@rules_cc//cc:cc_toolchain_config_lib.bzl",
    "feature",
    "feature_set",
    "flag_group",
    "flag_set",
    "tool_path",
    "variable_with_value",
    "with_feature_set",
)

def _impl(ctx):
    tool_paths = [ # NEW
        tool_path(
            name = "gcc",
            path = "/Users/josh.macdonald/src/ti-pru-cgt-2.3.3/bin/clpru"
        ),
        tool_path(
            name = "ld",
            path = "/Users/josh.macdonald/src/ti-pru-cgt-2.3.3/bin/clpru"
        ),
        tool_path(
            name = "ar",
            path = "/Users/josh.macdonald/src/ti-pru-cgt-2.3.3/bin/arpru"
        ),
        tool_path(
            name = "cpp",
            path = "/usr/bin/false",
        ),
        tool_path(
            name = "gcov",
            path = "/usr/bin/false",
        ),
        tool_path(
            name = "nm",
            path = "/usr/bin/false",
        ),
        tool_path(
            name = "objdump",
            path = "/usr/bin/false",
        ),
        tool_path(
            name = "strip",
            path = "/usr/bin/false",
        ),
    ]

    dependency_file_feature = feature(
        name = "dependency_file",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.c_compile,
                ],
                flag_groups = [
                    flag_group(
                        flags = ["-ppd=%{dependency_file}", "-ppa"],
                        expand_if_available = "dependency_file",
                    ),
                ],
            ),
        ],
    )

    include_paths_feature = feature(
        name = "include_paths",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.preprocess_assemble,
                    ACTION_NAMES.linkstamp_compile,
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                    ACTION_NAMES.cpp_header_parsing,
                    ACTION_NAMES.cpp_module_compile,
                    ACTION_NAMES.clif_match,
                    ACTION_NAMES.objc_compile,
                    ACTION_NAMES.objcpp_compile,
                ],
                flag_groups = [
                    flag_group(
                        flags = ["-I%{quote_include_paths}"],
                        iterate_over = "quote_include_paths",
                    ),
                    flag_group(
                        flags = ["-I%{include_paths}"],
                        iterate_over = "include_paths",
                    ),
                    flag_group(
                        flags = ["-I%{system_include_paths}"],
                        iterate_over = "system_include_paths",
                    ),
                ],
            ),
        ],
    )

    sysroot_feature = feature(
        name = "sysroot",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.preprocess_assemble,
                    ACTION_NAMES.linkstamp_compile,
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                    ACTION_NAMES.cpp_header_parsing,
                    ACTION_NAMES.cpp_module_compile,
                    ACTION_NAMES.cpp_module_codegen,
                    ACTION_NAMES.lto_backend,
                    ACTION_NAMES.clif_match,
                ],
                flag_groups = [
                    flag_group(
                        flags = ["-I=%{sysroot}/include"],
                        expand_if_available = "sysroot",
                    ),
                ],
            ),
        ],
    )
    
    random_seed_feature = feature(
        name = "random_seed",
        enabled = False,
    )
    
    features = [dependency_file_feature, include_paths_feature, random_seed_feature, sysroot_feature]
    
    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        builtin_sysroot = "/Users/josh.macdonald/src/ti-pru-cgt-2.3.3",
        cxx_builtin_include_directories = [
            # couldn't make this work, see sysroot_feature
        ],
        toolchain_identifier = "pru-toolchain",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "pru",
        target_libc = "unknown",
        compiler = "clpru",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
        features = features,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)

