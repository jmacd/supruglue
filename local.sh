#!/bin/sh

PRU=pru0
EXAMPLE=led_outin
REMOTEPROC=/sys/class/remoteproc/remoteproc1

REMOTE=./remote.sh
FIRMWARE=bazel-out/pru-fastbuild/bin/examples/${EXAMPLE}/${EXAMPLE}
SUPRUCTL=bazel-bin/tools/cmd/supructl/supructl_/supructl
BONE=${BONE:-beaglebone.local}

bazel build --config=${PRU} //examples/${EXAMPLE} || exit 1
bazel build --config=arm //tools/cmd/supructl || exit 1

echo "Copying"

scp -q -r -p ${REMOTE} ${FIRMWARE} ${SUPRUCTL} debian@${BONE}:

echo "Running"

ssh -q debian@${BONE} "${REMOTE} ${REMOTEPROC} ${EXAMPLE}"
