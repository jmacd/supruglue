#!/bin/sh

SCRIPT=

case ${1-rebuild} in
    build)
	SCRIPT=./build_deb.sh
	;;
    rebuild)
	SCRIPT=./tools/rebuild_deb.sh
	;;
    *)
	echo "usage: $0 [build|rebuild]"
	exit 1
	;;
esac

docker run \
    -v ${HOME}/src/supruglue:/supruglue \
    -ti \
    supruglue \
    /supruglue/kernel/docker/run.sh ${SCRIPT}
