#!/bin/bash

source ./header.sh

echo "[4] Dumping metadata (before) to $BEFORE..."
dumpe2fs $IMAGE > "$BEFORE"

echo "[5] Introducing controlled metadata corruption using debugfs..."

# 5A: For file1.txt, we'll erase its dirent, but keep the inode + bitmap intact
./file1-nodirent.sh

# 5B: For file2.txt, we'll erase the inode table contents, but keep the dirent and the block bitmap
./file2-noinode.sh

# 5C: For file3.txt, we'll unset the bit in the bitmap, but not change the inode table or dirent
./file3-noibit.sh

echo "[6] Checking dumpe2fs output on corrupt fs..."
dumpe2fs $IMAGE > "$INTERMEDIATE"
if [ ! -z $1 ]
then
   cp "$IMAGE" "$1"
fi

echo "[7] Running filesystem check + repair..."
e2fsck -fy $IMAGE -v > $FSCK_OUT
if [ -ne $? 1 ]
then
    echo "fsck failed"
    return 1
fi
    

echo "[8] Dumping metadata (after) to $AFTER..."
dumpe2fs $IMAGE > "$AFTER"

echo "Done."
echo "Compare with: diff -u $BEFORE $AFTER"
