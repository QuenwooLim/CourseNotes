#!/usr/bin/env bash
set -euo pipefail

# Simple benchmark runner: iterate process counts and matrix sizes
# Adjust PROCS and SIZES environment variables to customize

BIN=./parallel_matmul_mpi_v2
if [ ! -x "$BIN" ]; then
  echo "Binary $BIN not found or not executable. Run make first." >&2
  exit 1
fi

PROCS=${PROCS:-"1 2 4 9 16"}
SIZES=${SIZES:-"128 256 512 1024 2048"}
METHODS=${METHODS:-"coll p2p"}
PARTS=${PARTS:-"block cyclic"}
RUNS=${RUNS:-3}

OUT=bench_results.csv
echo "method,partition,procs,m,n,k,runs,time_s" > $OUT

for method in $METHODS; do
  for part in $PARTS; do
    for np in $PROCS; do
      for s in $SIZES; do
        echo "Running method=$method part=$part np=$np size=$s"
        # run once and capture last printed time line (allow running as root in container)
        out=$(mpirun --allow-run-as-root -np $np $BIN --m $s --n $s --k $s --runs $RUNS --method $method --partition $part --save 0 2>&1 | tail -n 1)
        # expected output contains time in parentheses or at end; store raw
        echo "$out"
        # append to CSV minimal fields
        # We'll try to parse time at the end after '='
        time=$(echo "$out" | sed -n 's/.*time(s)=//p')
        echo "$method,$part,$np,$s,$s,$s,$RUNS,$time" >> $OUT
      done
    done
  done
done

echo "Bench finished, results -> $OUT"
