#!/bin/bash
set -e

source ./header.sh

echo "[1] Creating image..."
truncate -s $SIZE $IMAGE

echo "[2] Creating ext4 filesystem..."
mkfs.ext4 -q $IMAGE

echo "[3] Initializing some files..."
sudo mount -t ext4 $IMAGE $MOUNT
cd $MOUNT
sudo sh -c "echo file1_contents > file1.txt"
sudo sh -c "echo file2_contents > file2.txt"
sudo sh -c "echo file3_contents > file3.txt"
sync
cd ..
sudo umount $MOUNT
