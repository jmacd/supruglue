# BeagleBone Black kernel & device tree

This directory builds the BeagleBone Black (am335x, armv7) kernel and device
trees that the PRU firmware in this repo targets. It is organized as two
tiers, both Docker-based:

| Tier | Path | Use it for | Speed |
| --- | --- | --- | --- |
| 1 (preferred) | [`devicetree/`](./devicetree) | Device tree (`.dtb`) and overlay (`.dtbo`) changes | seconds |
| 2 (fallback) | [`make.sh`](./make.sh) + [`docker/`](./docker) | A full kernel build, only when a CONFIG/driver change is required | many minutes |

Upstream sources are pinned as submodules:

- `BeagleBoard-DeviceTrees` - beagleboard/BeagleBoard-DeviceTrees @ `v6.18.x`
- `bb-kernel` - RobertCNelson/bb-kernel @ `am33x-v6.18` (mainline 6.18.x + `-bone`)

Both are kept **pristine**; supruglue's changes live as patches in this repo.
After cloning:

```
git submodule update --init kernel/BeagleBoard-DeviceTrees kernel/bb-kernel
```

> Almost all PRU peripheral work (ePWM, eCAP, EDMA, PRU interrupt routing) is a
> *device tree* change, not a kernel change. The stock BeagleBone Debian image
> (Debian 13.x IoT, v6.18.x) already ships the needed drivers, so reach for
> Tier 1 first and only fall back to Tier 2 if you truly need a new kernel
> CONFIG.

## Tier 1: device tree blobs (preferred)

Builds am335x DTBs/overlays from the pinned `BeagleBoard-DeviceTrees` sources
with supruglue's device-tree patches applied. No kernel tree is needed.

```
cd devicetree
./build.sh                                              # default target(s)
./build.sh src/arm/ti/omap/am335x-boneblack.dtb         # a base DTB
./build.sh src/arm/overlays/BB-EHRPWM1-P9_14-P9_16.dtbo # an overlay
```

Outputs land in `devicetree/out/`. Local device-tree patches go in
`devicetree/patches/` (see its README). `NATIVE=1 ./build.sh` builds with a
local `dtc`/`cpp` toolchain instead of Docker.

### Installing on the BeagleBone

Copy a blob into the running kernel's DTB directory and point the bootloader at
it:

```
# on the BeagleBone (uname -r gives the running version, e.g. 6.18.x-bone)
sudo cp am335x-boneblack.dtb /boot/dtbs/$(uname -r)/
```

To load an overlay instead of (or in addition to) a custom base DTB, enable the
u-boot overlay loader in the image's boot config (`/boot/uEnv.txt` or
`/boot/firmware/extlinux/extlinux.conf`, image-dependent), e.g.:

```
enable_uboot_overlays=1
uboot_overlay_addr0=/lib/firmware/BB-EHRPWM1-P9_14-P9_16.dtbo
```

then reboot. The BeagleBoard-DeviceTrees repo can also build and install
directly on-device via its own `./build_n_install.sh`.

## Tier 2: full kernel build (fallback only)

```
./make.sh build      # full build from scratch
./make.sh rebuild    # re-run after a config/source tweak
```

This builds a kernel `.deb` with `bb-kernel` inside Docker. The submodule is
synced into `kernel/.kernel-build/` (git-ignored) where the build runs and
caches the downloaded Linux source; artifacts appear under
`kernel/.kernel-build/deploy/`. The build is made non-interactive
(`AUTO_BUILD=1`) automatically. Use `docker/shell.sh` for an interactive shell
in the build container.

## Notes / history

- `KERNEL.diff` is the historical 5.10-era device-tree patch (against the old
  `am335x-bone-common-univ.dtsi` universal-cape tree, which no longer exists in
  6.18). It is kept only as reference for re-deriving the PRU enablement
  patches against the 6.18 tree (DTS moved to `arch/.../ti/omap/`, pinmux now
  uses `AM33XX_PADCONF` macros). New patches belong in `devicetree/patches/`.
