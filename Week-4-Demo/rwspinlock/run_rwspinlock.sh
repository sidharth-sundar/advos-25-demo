#!/bin/bash
make
sudo insmod sync_rwspinlock.ko
sleep 5
sudo rmmod sync_rwspinlock
sudo dmesg | grep -i SYNC_RWSPINLOCK | tail -n 30
