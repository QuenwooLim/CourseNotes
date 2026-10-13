#!/bin/bash

# Compile the program
make omp_matmul
if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

SCHEDULES=("default" "static" "dynamic")
SIZES=(128 256 512 1024 2048)
THREADS=(1 2 4 8 16)

RESULT_FILE="results.txt"
echo "# OpenMP Matrix Multiplication Benchmark Results" > "$RESULT_FILE"
echo "# Format: schedule size threads elapsed_seconds" >> "$RESULT_FILE"
echo "# Date: $(date)" >> "$RESULT_FILE"
echo "" >> "$RESULT_FILE"

for sched in "${SCHEDULES[@]}"; do
    for size in "${SIZES[@]}"; do
        for thr in "${THREADS[@]}"; do
            echo "Running: schedule=$sched size=$size threads=$thr"
            export OMP_NUM_THREADS=$thr
            output=$(./omp_matmul $size $size $size -s "$sched" -q 2>&1)
            elapsed=$(echo "$output" | grep "^ELAPSED:" | awk '{print $2}')
            echo "$sched $size $thr $elapsed" >> "$RESULT_FILE"
            echo "  -> ${elapsed}s"
        done
    done
done

echo ""
echo "All experiments done. Results saved to $RESULT_FILE"
