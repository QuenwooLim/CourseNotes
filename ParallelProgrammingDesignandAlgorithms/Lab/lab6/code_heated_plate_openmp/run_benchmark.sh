#!/bin/bash
#
# Benchmark script: compare OpenMP vs Pthreads (parallel_for) for heated plate
# Usage:
#   ./run_benchmark.sh              # print results to console
#   ./run_benchmark.sh --save       # also save CSV to results_YYYYMMDD_HHMMSS.csv
#

set -e

THREADS="1 2 4 8 16"
GRID=500

# ---- Parse args ----
SAVE_MODE=0
if [ "${1:-}" = "--save" ]; then
    SAVE_MODE=1
    TIMESTAMP=$(date +%Y%m%d_%H%M%S)
    SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
    OUT_CSV="${SCRIPT_DIR}/results_${TIMESTAMP}.csv"
fi

run_benchmark() {

echo "============================================="
echo " Building everything"
echo "============================================="
make clean 2>/dev/null || true
make all
echo ""

echo "============================================="
echo " Heated Plate Benchmark (grid ${GRID}x${GRID})"
echo "============================================="
echo ""

# CSV header
if [ "$SAVE_MODE" = "1" ]; then
    echo "method,threads,schedule,chunk,time_s,iterations" > "$OUT_CSV"
fi

run_one() {
    local method="$1" nt="$2" sched="$3" chunk="$4"
    local out wt it

    if [ "$method" = "OpenMP" ]; then
        export OMP_NUM_THREADS=$nt
        out=$(./heated_plate_openmp 2>&1)
    else
        local sched_flag=0
        [ "$sched" = "dynamic" ] && sched_flag=1
        out=$(./heated_plate_pthreads $nt $sched_flag $chunk 2>&1)
    fi

    wt=$(echo "$out" | grep "Wallclock time" | awk '{print $NF}')
    it=$(echo "$out" | grep -E "^\s+[0-9]+\s+[0-9]" | tail -1 | awk '{print $1}')

    local label
    if [ "$method" = "OpenMP" ]; then
        label="threads=$nt"
    elif [ "$sched" = "static" ]; then
        label="threads=$nt  schedule=static"
    else
        label="threads=$nt  schedule=dynamic  chunk=$chunk"
    fi
    echo "  $label  time=${wt}s  iterations=$it"

    if [ "$SAVE_MODE" = "1" ]; then
        echo "${method},${nt},${sched},${chunk},${wt},${it}" >> "$OUT_CSV"
    fi
}

# ---- OpenMP baseline ----
echo "--- OpenMP (original) ---"
for nt in $THREADS; do
    run_one "OpenMP" $nt "-" "-"
done
echo ""

# ---- Pthreads: static ----
echo "--- Pthreads (parallel_for) : Static scheduling ---"
for nt in $THREADS; do
    run_one "Pthreads" $nt "static" "-"
done
echo ""

# ---- Pthreads: dynamic ----
for chunk in 1 4 16; do
    echo "--- Pthreads (parallel_for) : Dynamic scheduling (chunk=$chunk) ---"
    for nt in $THREADS; do
        run_one "Pthreads" $nt "dynamic" $chunk
    done
    echo ""
done

echo "============================================="
echo " Benchmark complete"
echo "============================================="
}

# ---- Main ----
run_benchmark

if [ "$SAVE_MODE" = "1" ]; then
    echo ""
    echo "Results saved to: $OUT_CSV"
fi
