#!/bin/bash

OUTPUT_LINES=16
RESULTS_DIR=results

ops_per_thread=1000000
num_readers=5

if [ ! -z "$1" ]; then
    ops_per_thread=$1
fi

if [ ! -z "$2" ]; then
    ops_per_thread=$2
fi

mkdir ./${RESULTS_DIR}

make
sudo insmod sync_seqlock.ko ops_per_thread=${ops_per_thread} num_readers=${num_readers}
sleep 5
sudo rmmod sync_seqlock
sudo dmesg | grep -i SYNC_SEQLOCK | tail -n ${OUTPUT_LINES} > ./${RESULTS_DIR}/seqlock_ops-${ops_per_thread}_readers-${num_readers}.out
make clean

