#!/bin/bash
make
sudo insmod sync_spinlock.ko
sleep 5
sudo rmmod sync_spinlock
sudo dmesg | grep -i SYNC_SPINLOCK | tail -n 30
