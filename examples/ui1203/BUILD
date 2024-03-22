# -*- Mode: Python -*-
cc_binary(
    name = "ui1203",
    srcs = [
        "ui1203.c",
        ":irqgen",
    ],
    additional_linker_inputs = [
        "//tools/toolchain/am335x:linker_cmd",
    ],
    linkopts = [
        "$(location //tools/toolchain/am335x:linker_cmd)",
        "--stack_size=0x1000",
    ],
    deps = [
        "//lib/coroutine",
        "//lib/gpio",
        "//lib/initproc",
        "//lib/intc:service",
        "//lib/log/daemon",
        "//lib/resource",
        "//lib/rpmsg",
        "//lib/time:process",
        "//lib/ui1203",
        "//tools/toolchain/am335x",
    ],
)

genrule(
    name = "irqgen",
    srcs = [
        # Order is important: args[0] is all events
        # Remaining args define arriving system events.
        # (Only) one should set the host interrupt number.
        "//arch:sysevts",
        "//lib/rpmsg/am335x:irq",
        "//lib/time/am335x:iep_tim_cap_cmp_pend",
    ],
    outs = [
        "irq.c",
    ],
    cmd = "./$(location //tools/cmd/irqgen:irqgen) $(SRCS) > $@",
    tools = [
        "//tools/cmd/irqgen",
    ],
)
