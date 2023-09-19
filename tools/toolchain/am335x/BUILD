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
    copts = [
        "-Iexternal/ti-pru-support/include",
    ],
    deps = [
        "@ti-pru-support//include",
    ],
)
