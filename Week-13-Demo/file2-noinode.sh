#!/bin/bash
set -e
source ./header.sh

# Erase file1.txt dirent, keep inode table + inode bitmap
debugfs -w $IMAGE <<EOF
mi file2.txt
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
EOF
