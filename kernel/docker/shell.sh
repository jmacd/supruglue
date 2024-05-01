#!/bin/sh

docker run \
    -v ${HOME}/src/supruglue:/supruglue \
    -ti \
    supruglue \
    bash
