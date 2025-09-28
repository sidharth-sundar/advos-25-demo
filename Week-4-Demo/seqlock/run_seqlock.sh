#!/bin/bash
make
sudo insmod sync_seqlock.ko
sleep 5
sudo rmmod sync_seqlock
sudo dmesg | grep -i SYNC_SEQLOCK | tail -n 16
