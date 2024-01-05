#!/bin/sh

trap cleanup 1 2 3 6

PID=""

# TODO this is ...
ALL_GPIOs="gpio10 gpio11 gpio110 gpio111 gpio112 gpio113 gpio114 gpio115 gpio116 gpio117 gpio12 gpio13 gpio14 gpio15 gpio2 gpio20 gpio22 gpio23 gpio26 gpio27 gpio3 gpio30 gpio31 gpio32 gpio33 gpio34 gpio35 gpio36 gpio37 gpio38 gpio39 gpio4 gpio44 gpio45 gpio46 gpio47 gpio48 gpio49 gpio5 gpio50 gpio51 gpio60 gpio61 gpio62 gpio63 gpio65 gpio66 gpio67 gpio68 gpio69 gpio7 gpio70 gpio71 gpio72 gpio73 gpio74 gpio75 gpio76 gpio77 gpio78 gpio79 gpio8 gpio80 gpio81 gpio86 gpio87 gpio88 gpio89 gpio9"

#OUT_GPIOs="gpio117 gpio115"
OUT_GPIOs=${ALL_GPIOs}

IN_GPIOs="" 

LEDs="beaglebone:green:usr0 beaglebone:green:usr1 beaglebone:green:usr2 beaglebone:green:usr3"

cleanup()
{
  kill -9 $PID
  exit 1
}

configPins() {
    echo "Configuring user LEDs"
    for led in ${LEDs}; do
	echo none > /sys/class/leds/${led}/trigger
    done
    echo "Configuring GPIO pins"
    for gpio in ${OUT_GPIOs}; do
	echo out > /sys/class/gpio/${gpio}/direction
    done
    for gpio in ${IN_GPIOs}; do
	echo in > /sys/class/gpio/${gpio}/direction
    done
}

configPins

# echo "Stopping ..."
# echo stop > /sys/class/remoteproc/remoteproc1/state
# echo stop > /sys/class/remoteproc/remoteproc2/state

# mv example_pru0 /lib/firmware
# echo example_pru0 > /sys/class/remoteproc/remoteproc1/firmware

# echo "Starting ..."
# echo start > /sys/class/remoteproc/remoteproc1/state

# echo "State: ..."
# cat /sys/class/remoteproc/remoteproc1/state

mv supructl ./bin
mv two_leds  ./fw 

#./bin/supructl start  --remoteproc /sys/class/remoteproc/remoteproc2 --firmware ./fw/two_leds
./bin/supructl start  --remoteproc /sys/class/remoteproc/remoteproc1 --firmware ./fw/two_leds
#./bin/supructl rodata --firmware ./fw/example_pru0
