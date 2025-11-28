/scratch/sundar/advos-25-demo/Week-13-Demo#!/bin/bash

source ./header.sh

echo "[4] Dumping metadata (before) to $BEFORE..."
dumpe2fs $IMAGE > "$BEFORE"

echo "[5] Introducing controlled metadata corruption using debugfs..."

# 5A: Corrupt the atime timestamp of a known inode (inode 2 = root dir)
debugfs -w $IMAGE <<EOF
mi 2 atime=0x12345678
EOF

# 5B: Corrupt the free block count in a group descriptor (mismatched bitmap)
# We intentionally mark block 100 as allocated even though it is free.
debugfs -w $IMAGE <<EOF
setb 100
EOF

# 5C: Zero out part of the first block group's inode bitmap (simple corruption)
debugfs -w $IMAGE <<EOF
freei / 2
freei file1.txt 13
EOF

echo "[6] Checking dumpe2fs output on corrupt fs..."
dumpe2fs $IMAGE > "$INTERMEDIATE"
if [ ! -z $1 ]
then
   cp "$IMAGE" "$1"
fi

echo "[7] Running filesystem check + repair..."
e2fsck -fy $IMAGE -v > $FSCK_OUT

echo "[8] Dumping metadata (after) to $AFTER..."
dumpe2fs $IMAGE > "$AFTER"

echo "Done."
echo "Compare with: diff -u $BEFORE $AFTER"
