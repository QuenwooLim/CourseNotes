#!/usr/bin/env bash
set -euo pipefail

HERE=$(cd "$(dirname "$0")" && pwd)
cd "$HERE"

make -s

OUT1=discussion_mem_results.csv
echo "mode,N,block,avg_time_s,max_rss_mb" > $OUT1

SIZES=(512 1024)
BLOCKS=(64 128)
RUNS=1

for s in "${SIZES[@]}"; do
  for b in "${BLOCKS[@]}"; do
    echo "Running block test N=$s block=$b (naive)"
    line=$(./bin/block_matmul --size $s --block $b --mode naive --runs $RUNS | tail -n1)
    t=$(echo $line | sed -n 's/.*avg_time_s=\([0-9.eE+-]*\).*/\1/p')
    m=$(echo $line | sed -n 's/.*max_rss_mb=\([0-9.eE+-]*\).*/\1/p')
    echo "naive,$s,$b,$t,$m" >> $OUT1

    echo "Running block test N=$s block=$b (blocked)"
    line=$(./bin/block_matmul --size $s --block $b --mode blocked --runs $RUNS | tail -n1)
    t=$(echo $line | sed -n 's/.*avg_time_s=\([0-9.eE+-]*\).*/\1/p')
    m=$(echo $line | sed -n 's/.*max_rss_mb=\([0-9.eE+-]*\).*/\1/p')
    echo "blocked,$s,$b,$t,$m" >> $OUT1
  done
done

OUT2=discussion_sparse_results.csv
echo "N,density,avg_time_s,max_rss_mb" > $OUT2

SIZES_SPARSE=(1000 2000)
DENS=(0.01 0.001)
RUNS_SPARSE=1

for s in "${SIZES_SPARSE[@]}"; do
  for d in "${DENS[@]}"; do
    echo "Running sparse test N=$s density=$d"
    line=$(./bin/sparse_matmul --size $s --density $d --runs $RUNS_SPARSE | tail -n1)
    t=$(echo $line | sed -n 's/.*avg_time_s=\([0-9.eE+-]*\).*/\1/p')
    m=$(echo $line | sed -n 's/.*max_rss_mb=\([0-9.eE+-]*\).*/\1/p')
    echo "$s,$d,$t,$m" >> $OUT2
  done
done

echo "Discussion experiments finished. mem results: $OUT1, sparse results: $OUT2"
