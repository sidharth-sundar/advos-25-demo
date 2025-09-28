#!/bin/bash

OUTPUT_LINES=16
RESULTS_DIR=results

ops_per_thread=1000000
num_readers=5

if [ ! -z $1 ]; then
    ops_per_thread=$1
fi

if [ ! -z $2 ]; then
    num_readers=$2
fi

mkdir ./${RESULTS_DIR}

make > /dev/null
./sign_modules.sh > /dev/null
sudo insmod sync_rwspinlock.ko ops_per_thread=${ops_per_thread} num_readers=${num_readers}
sleep 5
sudo rmmod sync_rwspinlock
sudo dmesg | grep -i SYNC_RWSPINLOCK | tail -n ${OUTPUT_LINES} > ./${RESULTS_DIR}/rwspinlock_ops-${ops_per_thread}_readers-${num_readers}.out
make clean
