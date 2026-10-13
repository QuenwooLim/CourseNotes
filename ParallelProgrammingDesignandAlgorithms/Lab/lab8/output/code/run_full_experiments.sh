#!/bin/bash
# Full experiment runner for parallel shortest path
# Tests both Floyd-Warshall and Dijkstra-per-source with thread counts 1-16

PROG_FW="./apsp"
PROG_DIJ="./apsp_dijkstra"
DATA_DIR="requirement/网络数据集"
THREADS=(1 2 4 8 16)
TRIALS=5

# Build both programs
echo "=== Building ===" >&2
gcc -fopenmp -O3 -march=native -o apsp parallel_shortest_path.c -lm 2>&1
gcc -fopenmp -O3 -march=native -o apsp_dijkstra parallel_dijkstra_apsp.c -lm 2>&1
echo "Build done." >&2

run_trial() {
    local prog=$1
    local threads=$2
    local csv=$3
    local testfile=$4
    # Extract execution time value (second-to-last field of "Execution time" line)
    local result
    result=$(OMP_NUM_THREADS=$threads $prog "$csv" "$testfile" $threads 2>&1 | grep "Execution time" | awk '{print $4}')
    if [ -z "$result" ]; then
        # Try alternative parsing
        result=$(OMP_NUM_THREADS=$threads $prog "$csv" "$testfile" $threads 2>&1 | grep "Execution time" | sed 's/.*time: //' | sed 's/ seconds//')
    fi
    echo "$result"
}

echo "======================================================"
echo "  DATASET 1: Mouse (525 nodes, 14691 edges, deg~56)"
echo "======================================================"

echo ""
echo "--- Floyd-Warshall ---"
for t in "${THREADS[@]}"; do
    printf "Threads %2d: " $t
    for trial in $(seq 1 $TRIALS); do
        rt=$(run_trial $PROG_FW $t "$DATA_DIR/updated_mouse.csv" "test_queries_mouse.txt")
        printf "%s " "$rt"
    done
    echo ""
done

echo ""
echo "--- Dijkstra-per-Source ---"
for t in "${THREADS[@]}"; do
    printf "Threads %2d: " $t
    for trial in $(seq 1 $TRIALS); do
        rt=$(run_trial $PROG_DIJ $t "$DATA_DIR/updated_mouse.csv" "test_queries_mouse.txt")
        printf "%s " "$rt"
    done
    echo ""
done

echo ""
echo "======================================================"
echo "  DATASET 2: Flower (930 nodes, 13521 edges, deg~29)"
echo "======================================================"

echo ""
echo "--- Floyd-Warshall ---"
for t in "${THREADS[@]}"; do
    printf "Threads %2d: " $t
    for trial in $(seq 1 $TRIALS); do
        rt=$(run_trial $PROG_FW $t "$DATA_DIR/updated_flower.csv" "test_queries_flower.txt")
        printf "%s " "$rt"
    done
    echo ""
done

echo ""
echo "--- Dijkstra-per-Source ---"
for t in "${THREADS[@]}"; do
    printf "Threads %2d: " $t
    for trial in $(seq 1 $TRIALS); do
        rt=$(run_trial $PROG_DIJ $t "$DATA_DIR/updated_flower.csv" "test_queries_flower.txt")
        printf "%s " "$rt"
    done
    echo ""
done

echo ""
echo "=== Done ==="
