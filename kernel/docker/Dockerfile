# Image for the Tier 2 *fallback* full kernel build (RobertCNelson/bb-kernel,
# am33x-v6.18). The DTB-only workflow in ../devicetree is preferred for device
# tree iteration; build a full kernel only when a CONFIG/driver change is
# actually required.
FROM debian:trixie-slim

RUN apt-get -y update && \
    apt-get -y install --no-install-recommends \
        bc \
        bison \
        build-essential \
        ca-certificates \
        ccache \
        cpio \
        device-tree-compiler \
        fakeroot \
        flex \
        gcc-arm-linux-gnueabihf \
        git \
        kmod \
        libelf-dev \
        libncurses-dev \
        libssl-dev \
        lz4 \
        lzma \
        rsync \
        u-boot-tools \
        wget && \
    rm -rf /var/lib/apt/lists/*
