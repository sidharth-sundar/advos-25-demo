#!/bin/bash
set -e
source ./header.sh

# Erase file1.txt dirent, keep inode table + inode bitmap
debugfs -w $IMAGE <<EOF
freei file3.txt 15
EOF
