#!/bin/bash -e 

cd /supruglue/kernel/ti-linux-kernel-dev

echo Running $1 in `pwd`

# the command is passed in as $1
$1

echo Exited: $?
