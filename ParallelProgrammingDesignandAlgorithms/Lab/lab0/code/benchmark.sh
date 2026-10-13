#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

OUT=results.csv
echo "m,n,k,version,time,rel_speed_vs_prev,abs_speed_vs_v1,gflops,peak_percent,checksum" > "$OUT"

if [ "${ALL-0}" = "1" ]; then
  TESTS=("512,512,512" "1024,1024,1024" "2048,2048,2048" "2048,512,1024" "512,2048,1024" "1024,512,2048")
  TESTS+=("1536,1536,1536")
else
  TESTS=("1024,1024,1024")
fi

VERSIONS=("Python_py" "Python" "C_O0" "IJK" "C_O3" "Unroll" "Block" "C_SIMD" "C_OMP" "BLAS")
V2=("Python_py" "Python" "C_O0" "IJK" "C_O3" "Unroll" "Block" "C_SIMD" "C_OMP" "BLAS")

CMD_Python='python3 matrix.py'
CMD_Python_PY='python3 matrix.py'
CMD_C_O0='./serial_O0'
CMD_IJK='./serial_ijk'
CMD_C_O3='./serial_O3'
CMD_Unroll='./serial_unroll'
CMD_BLAS='./serial_blas'
CMD_Block='./serial_block'
CMD_C_SIMD='./serial_simd'
CMD_C_OMP='./serial_omp'

PEAK_GFLOPS=3072.0

for T in "${TESTS[@]}"; do
  IFS=, read -r m n k <<< "$T"
  declare -A times
  declare -A checksums

  out=""
  out=$($CMD_Python $m $n $k numpy 2>/dev/null || true)
  if [ -n "$out" ]; then read mm nn kk t c <<< "$out"; times[Python]=$t; checksums[Python]=$c; else times[Python]=""; checksums[Python]=""; fi

  out=""
  out=$($CMD_Python_PY $m $n $k py 2>/dev/null || true)
  if [ -n "$out" ]; then read mm nn kk t c <<< "$out"; times[Python_py]=$t; checksums[Python_py]=$c; else times[Python_py]=""; checksums[Python_py]=""; fi

  out=""
  if [ -x "$CMD_C_O0" ]; then out=$($CMD_C_O0 $m $n $k 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk t c <<< "$out"; times[C_O0]=$t; checksums[C_O0]=$c; else times[C_O0]=""; checksums[C_O0]=""; fi

  out=""
  if [ -x "$CMD_IJK" ]; then out=$($CMD_IJK $m $n $k 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk t c <<< "$out"; times[IJK]=$t; checksums[IJK]=$c; else times[IJK]=""; checksums[IJK]=""; fi

  out=""
  if [ -x "$CMD_C_O3" ]; then out=$($CMD_C_O3 $m $n $k 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk t c <<< "$out"; times[C_O3]=$t; checksums[C_O3]=$c; else times[C_O3]=""; checksums[C_O3]=""; fi

  out=""
  if [ -x "$CMD_Unroll" ]; then out=$($CMD_Unroll $m $n $k 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk t c <<< "$out"; times[Unroll]=$t; checksums[Unroll]=$c; else times[Unroll]=""; checksums[Unroll]=""; fi

  out=""
  if [ -x "$CMD_BLAS" ]; then out=$($CMD_BLAS $m $n $k 2>/dev/null || true); else out=$($CMD_Python $m $n $k numpy 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk t c <<< "$out"; times[BLAS]=$t; checksums[BLAS]=$c; else times[BLAS]=""; checksums[BLAS]=""; fi

  out=""
  if [ -x "$CMD_Block" ]; then out=$($CMD_Block $m $n $k 64 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk B t c <<< "$out"; times[Block]=$t; checksums[Block]=$c; else times[Block]=""; checksums[Block]=""; fi

  out=""
  if [ -x "$CMD_C_SIMD" ]; then out=$($CMD_C_SIMD $m $n $k 64 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk B t c <<< "$out"; times[C_SIMD]=$t; checksums[C_SIMD]=$c; else times[C_SIMD]=""; checksums[C_SIMD]=""; fi

  out=""
  if [ -x "$CMD_C_OMP" ]; then out=$($CMD_C_OMP $m $n $k 64 2>/dev/null || true); fi
  if [ -n "$out" ]; then read mm nn kk B t c <<< "$out"; times[C_OMP]=$t; checksums[C_OMP]=$c; else times[C_OMP]=""; checksums[C_OMP]=""; fi

  prev_time=""
  time_v1=${times[Python]}
  for ver in "${V2[@]}"; do
    t=${times[$ver]}
    c=${checksums[$ver]}
    if [ -z "$t" ]; then continue; fi
    flops=$(awk -v a=$m -v b=$n -v c=$k 'BEGIN{printf "%.0f", 2*a*b*c}')
    gflops=$(awk -v f=$flops -v tt=$t 'BEGIN{printf "%.6f", (f/(tt*1e9))}')
    if [ -z "$prev_time" ]; then rel=1.0; else rel=$(awk -v a=$prev_time -v b=$t 'BEGIN{printf "%.4f", (a/b)}'); fi
    if [ -z "$time_v1" ]; then abs=1.0; else abs=$(awk -v a=$time_v1 -v b=$t 'BEGIN{printf "%.4f", (a/b)}'); fi
    peakp=$(awk -v g=$gflops -v p=$PEAK_GFLOPS 'BEGIN{printf "%.2f", (g/p*100)}')
    echo "$m,$n,$k,$ver,$t,$rel,$abs,$gflops,$peakp,$c" >> "$OUT"
    prev_time=$t
  done

done

echo "Wrote $OUT"
