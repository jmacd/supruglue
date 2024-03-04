# -*- Mode: Python -*-
cc_binary(
    name = "pwm_outin",
    srcs = [
        "pwm_outin.c",
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
        "//lib/rpmsg",
        "//lib/pwm",
        "//lib/time:process",
        "//lib/resource",
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
        "//lib/pwm/am335x:epwm0_intr_pend",
        "//lib/pwm/am335x:epwm1_intr_pend",
        "//lib/pwm/am335x:epwm2_intr_pend",
    ],
    outs = [
        "irq.c",
    ],
    cmd = "./$(location //tools/cmd/irqgen:irqgen) $(SRCS) > $@",
    tools = [
        "//tools/cmd/irqgen",
    ],
)
