#!/bin/sh

docker run \
    -v ${HOME}/src/supruglue:/supruglue \
    -ti \
    supruglue \
    /supruglue/kernel/compile.sh


# docker run \                                                                      ✔  21:10:34 
#     -v ${HOME}/src/supruglue/kernel:/supruglue \
#     -ti \
#     supruglue \
#     bash

