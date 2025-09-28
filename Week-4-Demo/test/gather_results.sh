#!/bin/bash

BASE_DIR=$(realpath ..);
CURRENT_DIR=$(pwd)
RESULTS_DIR="results"
LOCAL_RESULTS_DIR="${CURRENT_DIR}/${RESULTS_DIR}"

SYNC_DIRS=(race spinlock rwspinlock seqlock rcu)
READER_COUNTS=(1 5 9)
OPS=(1000000)

mkdir -p ${LOCAL_RESULTS_DIR}

for dirname in ${SYNC_DIRS[@]}; do
    cd "${BASE_DIR}/${dirname}"
    for ops in ${OPS[@]}; do
	for readers in ${READER_COUNTS[@]}; do
	    cp ${RESULTS_DIR}/"${dirname}_ops-${ops}_readers-${readers}.out" ${LOCAL_RESULTS_DIR}
	done	
    done
done


