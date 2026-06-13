#!/usr/bin/env bash
#
# Tier 1: build am335x device tree blobs (and overlays) for the BeagleBone
# Black from the pinned BeagleBoard-DeviceTrees submodule, with supruglue's
# local device-tree patches applied.
#
# The submodule is never modified: its sources are copied into a scratch
# directory, patched there, and built. Results land in ./out/.
#
# Usage:
#   ./build.sh [target ...]
#
# With no arguments the default targets (see DEFAULT_TARGETS) are built.
# Targets are paths relative to the BeagleBoard-DeviceTrees root, e.g.
#   ./build.sh src/arm/ti/omap/am335x-boneblack.dtb
#   ./build.sh src/arm/overlays/BB-EHRPWM1-P9_14-P9_16.dtbo
#
# Set NATIVE=1 to build directly with a local dtc/cpp/make toolchain instead
# of Docker.
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
KERNEL_DIR="$(dirname "$HERE")"
DT_SUBMODULE="$KERNEL_DIR/BeagleBoard-DeviceTrees"
IMAGE="supruglue-devicetree"

DEFAULT_TARGETS=(
  "src/arm/ti/omap/am335x-boneblack.dtb"
)

if [ "$#" -gt 0 ]; then
  TARGETS=("$@")
else
  TARGETS=("${DEFAULT_TARGETS[@]}")
fi

if [ ! -e "$DT_SUBMODULE/Makefile" ]; then
  echo "error: $DT_SUBMODULE is missing; run: git submodule update --init" >&2
  exit 1
fi

# The actual build steps, run either natively or inside the container. Expects
# the kernel directory mounted/visible at $WORK.
build_body() {
  local work="$1"; shift
  local scratch="$work/devicetree/.build"
  local out="$work/devicetree/out"

  rm -rf "$scratch"
  mkdir -p "$scratch" "$out"

  # Copy pristine sources out of the submodule (never patch it in place).
  rsync -a --exclude='.git' "$work/BeagleBoard-DeviceTrees/" "$scratch/"

  # Apply supruglue device-tree patches, if any.
  shopt -s nullglob
  local patches=("$work/devicetree/patches"/*.patch)
  shopt -u nullglob
  if [ "${#patches[@]}" -gt 0 ]; then
    echo "Applying ${#patches[@]} patch(es):"
    local p
    for p in "${patches[@]}"; do
      echo "  $(basename "$p")"
      ( cd "$scratch" && git apply --whitespace=nowarn "$p" )
    done
  else
    echo "No patches in devicetree/patches/ (building pristine sources)."
  fi

  echo "Building: $*"
  make -C "$scratch" "$@"

  # Collect the built blobs.
  local t name
  for t in "$@"; do
    name="$(basename "$t")"
    cp -v "$scratch/$t" "$out/$name"
  done
  echo "Output in devicetree/out/"
}

if [ "${NATIVE:-0}" = "1" ]; then
  build_body "$KERNEL_DIR" "${TARGETS[@]}"
  exit 0
fi

echo "Building Docker image $IMAGE ..."
docker build -t "$IMAGE" "$HERE" >/dev/null

# Re-run this script's build_body inside the container with the kernel dir
# mounted at /work.
docker run --rm \
  -v "$KERNEL_DIR":/work \
  -w /work/devicetree \
  "$IMAGE" \
  bash -c "$(declare -f build_body); build_body /work $(printf '%q ' "${TARGETS[@]}")"
