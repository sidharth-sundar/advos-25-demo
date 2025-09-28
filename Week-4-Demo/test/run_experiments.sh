#!/bin/bash

BASE_DIR=$(realpath ..);

SYNC_DIRS=(race spinlock)
#SYNC_DIRS=(race spinlock rwspinlock seqlock rcu)
READER_COUNTS=(1 5 9)
OPS=(1000000)

for dirname in ${SYNC_DIRS[@]}; do
    cd "${BASE_DIR}/${dirname}"
    for ops in ${OPS[@]}; do
	for readers in ${READER_COUNTS[@]}; do
	    bash run_${dirname}.sh ${ops} ${readers}
	done	
    done
done


