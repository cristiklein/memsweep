#!/bin/bash

# Experiment verbs
set_latency() {
        echo "Setting latency to $1 ns" >&2 
        echo $1 > /sys/block/zram0/page_rw_latency_ns # 1000 ns, approx 4 MB/s
}

cleanup() {
        swapoff -a || true # status code is irrelevant
        rmmod zram || true # may fail
}

set -e

if [ `id -u` != 0 ]; then
        echo "Run me with sudo"
        exit 1
fi

cleanup
modprobe zram

echo null > /sys/block/zram0/comp_algorithm
echo 24g > /sys/block/zram0/disksize
echo 1 > /sys/block/zram0/disable_zero_page_optimization

dd if=/dev/zero bs=4096 iflag=fullblock count=6291456 | pv | dd bs=4096 of=/dev/zram0 oflag=direct

mkswap /dev/zram0
swapon /dev/zram0

make

set_latency 1000
./memsweep -s 5g -i 5

set_latency 1000
./memsweep -s 10g -i 5

set_latency 100
./memsweep -s 10g -i 5

set_latency 10
./memsweep -s 10g -i 5

set_latency 0
./memsweep -s 10g -i 5

cleanup
