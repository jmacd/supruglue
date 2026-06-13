# Device tree patches

Drop `git format-patch` style `*.patch` files here to customize the BeagleBone
Black device tree. They are applied (in sorted order) to a pristine copy of the
`kernel/BeagleBoard-DeviceTrees` submodule by `../build.sh`; the submodule
itself is never modified.

Patches apply from the repository root, so paths look like
`src/arm/ti/omap/am335x-...`.

These will hold the supruglue PRU enablement (PRU interrupt-controller host
reservation, EDMA reserved slots, eCAP, etc.). The historical 5.10-era source
for these changes is `../../KERNEL.diff` (must be re-derived against the 6.18
tree, whose DTS moved to `arch/.../ti/omap/` and uses `AM33XX_PADCONF` macros).
