# -*- Mode: Python -*-
cc_binary(
    name = "two_leds",
    srcs = [
        "two_leds.c",
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
        "//tools/toolchain/am335x",
    ],
)

genrule(
    name = "irqgen",
    srcs = [
        # Order is important: args[0] is all events
        # Remaining args define arriving system events.
        # (Only) one should set the host.
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
