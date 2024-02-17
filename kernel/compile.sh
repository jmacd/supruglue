#!/bin/bash -e 

cd /supruglue/kernel/ti-linux-kernel-dev

echo Running build_deb.sh in `pwd`

./build_deb.sh

echo Exited: build_deb.sh: $?
