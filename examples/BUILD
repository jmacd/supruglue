# -*- Mode: Python -*-
cc_binary(
    name = "example_pru0",
    srcs = [
        "example.c",
        ":irqgen",
    ],
    additional_linker_inputs = [
        "//tools/toolchain/am335x:pru0_cmd",
    ],
    linkopts = [
        "$(location //tools/toolchain/am335x:pru0_cmd)",
        "--stack_size=0x1000",
    ],
    deps = [
        "//lib/coroutine",
        "//lib/gpio",
        "//lib/initproc",
        "//lib/log/daemon",
        "//lib/rpmsg",
        "//lib/time:process",
        "//lib/intc:service",
        "//tools/toolchain/am335x",
    ],
)

# cc_binary(
#     name = "local",
#     srcs = ["local.c"],
#     deps = [
#         "//lib/coroutine",
#     ],
# )

genrule(
    name = "irqgen",
    srcs = [
        "//tools/toolchain/am335x:pru0_irq",
        "//lib/time/am335x:iep_tim_cap_cmp_pend",
        "//arch:sysevts",
    ],
    outs = [
        "rsc.c",
    ],
    cmd = "./$(location //tools/cmd/irqgen:irqgen) $(location //arch:sysevts) $(location //tools/toolchain/am335x:pru0_irq) $(location //lib/time/am335x:iep_tim_cap_cmp_pend) > $@",
    tools = ["//tools/cmd/irqgen"],
)
