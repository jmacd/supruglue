package(default_visibility = ["//visibility:public"])

filegroup(
    name = "cmd",
    srcs = [
	    "AM335x_PRU_intc_rscTbl.cmd",
   ],
)

cc_library(
    name = "runtime",
    srcs = [
        "@ti-cgt-pru//:libc_a",
    ],
    deps = [
        "@ti-pru-support//:rpmsg",
	"@ti-pru-support//:am335x",
    ],
)

