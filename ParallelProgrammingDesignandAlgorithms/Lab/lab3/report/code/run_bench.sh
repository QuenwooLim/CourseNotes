#!/usr/bin/env bash
set -euo pipefail

# Bench script for Pthreads programs: matmul_pthreads and sum_pthreads

BASE_DIR=$(dirname "$0")
cd "$BASE_DIR"

MATBIN=./matmul_pthreads
SUMBIN=./sum_pthreads

if [ ! -x "$MATBIN" ] || [ ! -x "$SUMBIN" ]; then
  echo "Binaries not found. Run 'make' first." >&2
  exit 1
fi

PROCS=${PROCS:-"1 2 4 8 16"}
SIZES=${SIZES:-"128 256 512 1024 2048"}
SUM_SIZES=${SUM_SIZES:-"1000000 4000000 16000000 64000000 128000000"}
MAT_PARTS=${MAT_PARTS:-"block cyclic block_col"}
SUM_AGGS=${SUM_AGGS:-"serial mutex atomic"}
RUNS=${RUNS:-3}

OUT_M=results_matmul.csv
OUT_S=results_sum.csv

echo "partition,threads,m,n,k,runs,time_s,threadMax_s" > $OUT_M
echo "partition,agg,threads,n,runs,time_s,sum" > $OUT_S

for part in $MAT_PARTS; do
  for threads in $PROCS; do
    for s in $SIZES; do
      echo "matmul: part=$part threads=$threads size=$s"
      out=$($MATBIN --m $s --n $s --k $s --threads $threads --runs $RUNS --partition $part 2>&1 | tail -n 1)
      time=$(echo "$out" | sed -n 's/.*time(s)=\([0-9.]*\).*/\1/p')
      threadMax=$(echo "$out" | sed -n 's/.*threadMax(s)=\([0-9.]*\).*/\1/p')
      echo "$part,$threads,$s,$s,$s,$RUNS,$time,$threadMax" >> $OUT_M
    done
  done
done
for part in block cyclic; do
  for agg in $SUM_AGGS; do
    for threads in $PROCS; do
      for n in $SUM_SIZES; do
        echo "sum: part=$part agg=$agg threads=$threads n=$n"
        out=$($SUMBIN --n $n --threads $threads --runs $RUNS --partition $part --agg $agg 2>&1 | tail -n 1)
        time=$(echo "$out" | sed -n 's/.*time(s)=\([0-9.]*\).*/\1/p')
        sum=$(echo "$out" | sed -n 's/.*sum=\([0-9]*\).*/\1/p')
        echo "$part,$agg,$threads,$n,$RUNS,$time,$sum" >> $OUT_S
      done
    done
  done
done

echo "Bench finished. Results: $OUT_M and $OUT_S"
