#!/bin/bash
set -e
source ./header.sh

# Erase file1.txt dirent, keep inode table + inode bitmap
debugfs -w $IMAGE <<EOF
cd /
unlink file1.txt
EOF
