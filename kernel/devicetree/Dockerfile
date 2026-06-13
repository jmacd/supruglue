# Minimal image for building am335x device tree blobs from the
# BeagleBoard-DeviceTrees sources. No kernel tree is required: the repo ships
# its own dt-bindings headers and builds with dtc + cpp.
FROM debian:trixie-slim

RUN apt-get -y update && \
    apt-get -y install --no-install-recommends \
        device-tree-compiler \
        cpp \
        gcc \
        make \
        rsync \
        git && \
    rm -rf /var/lib/apt/lists/*
