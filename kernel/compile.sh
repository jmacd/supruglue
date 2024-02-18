#!/bin/bash -e 

cd /supruglue/kernel/ti-linux-kernel-dev

echo Running build_deb.sh in `pwd`

#./build_kernel.sh
#./build_deb.sh
./tools/rebuild_deb.sh

echo Exited: build_deb.sh: $?
