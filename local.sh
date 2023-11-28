#!/bin/sh

REMOTE=./remote.sh
FIRMWARE=bazel-out/pru-fastbuild/bin/examples/example_pru0
SUPRUCTL=bazel-bin/tools/cmd/supructl/supructl_/supructl

BONE=${BONE:-beaglebone.local}

bazel build --config=pru0 //examples:example_pru0 || exit 1
bazel build --config=arm //tools/cmd/supructl || exit 1

echo "Copying"

scp -q -r -p ${REMOTE} ${FIRMWARE} ${SUPRUCTL} debian@${BONE}:

echo "Running"

ssh -q debian@${BONE} ${REMOTE}
