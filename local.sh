#!/bin/sh

BONE=beaglebone.local

scp -q -r -p remote.sh bazel-out/pru-fastbuild/bin/examples/example_pru0 debian@${BONE}:

ssh -q debian@${BONE} './remote.sh'
