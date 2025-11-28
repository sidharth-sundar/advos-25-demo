This tutorial is for the CS 477 Fall 2025 class. 

## How to Run
First, configure the global parameters in `header.sh`. Set the file size (`SIZE`) and mount path (`MOUNT`) as you please.

`init_fs.sh` initializes a file `IMAGE` of size `SIZE` and writes 3 files to the root directory.

`corrupt-ext4-full.sh` runs the full suite of corruptions displayed in the tutorial. Each of the 3 files has a corresponding metadata corruption, detailed in the corresponding `file<n>-no<corrpution>.sh` scripts.

`corrupt-ext4-extra.sh` performs some more mutations that e2fsck can detect.

Both `corrupt-` scripts internally call `fsck` to repair the corrupted disk image (fsck output stored in `FSCK_OUT`), and use `dumpe2fs` to store the initial, post-corruption, and post-repair metadata in corresponding .txt files (`BEFORE`, `INTERMEDIATE`, and `AFTER`).

`mount.sh` and `umount.sh` are utilities to mount `IMAGE` at the mount point `MOUNT` to inspect the files and to unmount it, respectively.

`cleanup.sh` ensures the file system is unmounted, then erases the disk image.