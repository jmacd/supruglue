#!/usr/bin/env bash
#
# Open an interactive shell in the Tier 2 kernel-build container with the
# kernel directory mounted at /work.
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
KERNEL_DIR="$(dirname "$HERE")"
IMAGE="supruglue-kernel"

docker build -t "${IMAGE}" "${HERE}"
docker run --rm -ti -v "${KERNEL_DIR}":/work -w /work "${IMAGE}" bash
