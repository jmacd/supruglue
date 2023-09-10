#!/bin/sh

# This would be a little more elegant using a Dockerfile!
# Probably Bazel could do even better.

SOURCE=https://dr-download.ti.com/software-development/ide-configuration-compiler-or-debugger/MD-FaNNGkDH7s/2.3.3/ti_cgt_pru_2.3.3_linux_installer_x86.bin
SUM=abb76fac986993aafaf467915985ec4f
INPUT=$(basename $SOURCE)
OUTPUT=$(basename $SOURCE .bin).tar.gz

if [ ! -f ${INPUT} ]; then
    curl ${SOURCE} -o ${INPUT}
fi

EXPECT="${SUM}  ${INPUT}"
CHECK=`md5sum ${INPUT}`
if [ "${CHECK}" = "${EXPECT}" ];  then
    echo md5sum correct!
else
    echo md5sum mismatch!
    echo expected "${EXPECT}"
    echo got      "${CHECK}"
    exit
fi

chmod +x ${INPUT}

PID=`docker run -d -it --platform linux/amd64 --mount type=bind,source="$(pwd)",target=/data debian /bin/sh`

echo Started docker ${PID}

# Update package lists
docker exec -ti ${PID} /usr/bin/apt-get update

# Allow 32bit binaries to run
docker exec -ti ${PID} /usr/bin/apt-get install -y gcc-multilib

# Run the installer
docker exec -ti ${PID} bash -c "/data/${INPUT} << EOM
/ti-cgt-pru
Y
EOM
"

docker exec -ti ${PID} tar -C /ti-cgt-pru -czvf /data/${OUTPUT}.tmp {include,lib}
docker exec -ti ${PID} mv /data/${OUTPUT}.tmp /data/${OUTPUT}

trap "docker rm -f ${PID}" EXIT


