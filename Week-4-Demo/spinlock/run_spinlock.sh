#!/bin/bash

OUTPUT_LINES=16
RESULTS_DIR=results

ops_per_thread=1000000
num_readers=5
 
if [ ! -z "$1" ]; then
    ops_per_thread=$1
fi
 
if [ ! -z "$2" ]; then
    num_readers=$2
fi
 
mkdir ./${RESULTS_DIR}

make
sudo insmod sync_spinlock.ko
sleep 5
sudo rmmod sync_spinlock
sudo dmesg | grep -i SYNC_SPINLOCK | tail -n ${OUTPUT_LINES} > ./${RESULTS_DIR}/spinlock_ops-${ops_per_thread}_readers-${num_readers}.out
make clean
