#!/usr/bin/env bash
#
# Tier 2 fallback: build a full BeagleBone Black kernel .deb with
# RobertCNelson/bb-kernel (am33x-v6.18) in Docker.
#
# Prefer the lightweight DTB workflow in ./devicetree for device-tree changes;
# only build a full kernel when you need a CONFIG/driver change.
#
# Usage:
#   ./make.sh [build|rebuild]
#     build   - full build from scratch  (bb-kernel/build_deb.sh)
#     rebuild - re-run after a config/source tweak (tools/rebuild_deb.sh)
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
IMAGE="supruglue-kernel"

case "${1:-build}" in
  build)   SCRIPT="./build_deb.sh" ;;
  rebuild) SCRIPT="./tools/rebuild_deb.sh" ;;
  *) echo "usage: $0 [build|rebuild]" >&2; exit 1 ;;
esac

docker build -t "${IMAGE}" "${HERE}/docker"
docker run --rm -ti \
  -v "${HERE}":/work \
  "${IMAGE}" \
  /work/docker/run.sh "${SCRIPT}"
