This directory includes Docker support for building Beaglbone Black kernels.

https://github.com/RobertCNelson/ti-linux-kernel-dev

## Build a docker image

```
(cd ./docker && make)
```

## Build a kernel .deb

```
./make.sh build
```

## (Re-)build a kernel .deb

```
./make.sh rebuild
```

## TODO

Need to edit `system.sh` with:

```
AUTO_BUILD=1
```

This prevents running `make menuconfig` and clobbering your config.
??
