#!/usr/bin/env bash
set -euo pipefail


HERE=$(cd "$(dirname "$0")" && pwd)
cd "$HERE"

make -s

LAUNCHER="mpiexec"
PY=./bin/parallel_matmul_p2p

PROCS_LIST=(1 2 4 9 16)
SIZES=(128 256 512 1024 2048)
RUNS=2

OUT=results_c_full.csv
echo "procs,size,avg_time_s" > "$OUT"

for p in "${PROCS_LIST[@]}"; do
  for s in "${SIZES[@]}"; do
    echo "Running C parallel_matmul_p2p procs=$p size=$s"
    OMPI_ALLOW_RUN_AS_ROOT=1 OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1 $LAUNCHER -n $p $PY --size $s --runs $RUNS 2>&1 | tee /tmp/cbench_out.txt
    line=$(tail -n 1 /tmp/cbench_out.txt || true)
    t=$(echo "$line" | sed -n 's/.*approx_time(s)=\([0-9.eE+-]*\).*/\1/p' || true)
    if [ -z "$t" ]; then t="NA"; fi
    echo "$p,$s,$t" >> "$OUT"
  done
done

echo "C benchmark finished. Results in $OUT"
