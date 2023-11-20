## Supruglue

Supruglue is a development environment and operating system for the
Texas Instruments Programmable Realtime Unit for industrial
communications on the Beaglebone Black.  This project consists of:

- Bazel crosstool definition for using Texas Instruments PRU Code Generation tools and pru-software-support-package
- Cooperative threading using setjmp/longjump
- Diagnostic logging over RPMsg, debugging support
- Test harness using C++ Googletest library
- Code generation for pin definitions, system events, interrupt definitions, etc.
- Command-line tool to easily start, stop, and inspect PRU firmware.

### Why Supruglue

At first I struggled to learn the Texas Instruments am335x
system-on-a-chip.  There are a number of gotchas that can easily
derail a new developer.  Where there are examples, they are mostly out
of date, and generally, you really have to want to use these things to
achieve success.

In 2021, after nearly twenty years working for Big Tech companies I
moved to the Mendocino coast and became the owner/operator of the
Caspar, California community water system.  With a background in
open-source software and specializing in telemetry systems, you
guessed it--

I'm writing Supruglue as the foundation for an open-source SCADA
system.

### WORK IN PROGRESS

This repository is under development.

The next steps are:

- Time support: timestamps, sleep & timer functions
- Carveout support: setup regions for bulk data transfer
- Interrupt controller logic
- Drivers: eCAP, PWM, DMA, etc.
- Multi-PRU coordination.

Sample application goals:

- Pulse counter repeater: count and repeat pulses between a water meter and dosing pump, connect to online monitoring
- UI1203 meter reader: read UI1203 protocol, connect to online monitoring.

If you made it this far, you're probably thinking what I'm thinking.
Code size is a concern!  The current example application has around
4KiB of program text, which is half the available instruction space.

As space becomes tight, more space-optimization will likely be needed.

### Quickstart

To test the PRU build w/ TI's compiler:

```
bazel build --config=clpru ...
```

To test using a host toolchain:

```
bazel test --config=host ...
```

To cross-compile for the Beaglebone ARM:
```
bazel build --config=arm ...
```

### Example

The example will alternatingly flash two LEDs on pins `P9_23` and `P9_25`.

```
bazel build --config=clpru //examples:example_pru0
```

The firmware is located in `bazel-out/pru-fastbuild/bin/examples/example_pru0`

To build the Supruglue command-line interface,

```
bazel build --config=arm //tools/cmd/supructl
```

The program is located in `bazel-bin/tools/cmd/supructl/supructl_/supructl`.

Copy both to the Beaglebone Black machine, then:

```
supructl start --firmware example_pru0
```

The example starts two threads (one for each LED) in addition to the
system `init` thread, a system logging `daemon` thread, and a thread
busy writing log statements.

### Get in touch!

Feel free to file an issue or start a discussion.

Find me working on the [OpenTelemetry project](https://opentelemetry.io/), 
where I am a member of the technical committee, or in the CNCF slack.
