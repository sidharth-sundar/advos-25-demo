#!/bin/bash
make
sudo insmod sync_rcu.ko
sleep 5
sudo rmmod sync_rcu
sudo dmesg | grep -i SYNC_RCU | tail -n 16
