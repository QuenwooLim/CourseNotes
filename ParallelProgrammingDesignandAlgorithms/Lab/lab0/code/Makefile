CC = gcc
BASE_CFLAGS = -std=c11
OPT_FLAGS = -O3 -march=native -funroll-loops

all: serial_O3 serial_O0 serial_ijk serial_unroll serial_block serial_simd serial_omp serial_blas

serial_O3: serial.c
	$(CC) $(BASE_CFLAGS) $(OPT_FLAGS) -o serial_O3 serial.c -lm

serial_O0: serial.c
	$(CC) $(BASE_CFLAGS) -O0 -o serial_O0 serial.c -lm

serial_ijk: serial_ijk.c
	$(CC) $(BASE_CFLAGS) $(OPT_FLAGS) -o serial_ijk serial_ijk.c -lm

serial_unroll: serial_unroll.c
	$(CC) $(BASE_CFLAGS) $(OPT_FLAGS) -o serial_unroll serial_unroll.c -lm

serial_simd: serial_simd.c
	$(CC) $(BASE_CFLAGS) $(OPT_FLAGS) -o serial_simd serial_simd.c -lm

serial_omp: serial_omp.c
	$(CC) $(BASE_CFLAGS) $(OPT_FLAGS) -fopenmp -o serial_omp serial_omp.c -lm -fopenmp

serial_blas: serial_blas.c
	-@$(CC) $(BASE_CFLAGS) -DUSE_BLAS -O3 -o serial_blas serial_blas.c -lopenblas -lcblas -lblas -lm 2>/dev/null || \
	 $(CC) $(BASE_CFLAGS) -DUSE_BLAS -O3 -o serial_blas serial_blas.c -lopenblas -lblas -lm 2>/dev/null || \
	 $(CC) $(BASE_CFLAGS) -DUSE_BLAS -O3 -o serial_blas serial_blas.c -lmkl_rt -lm 2>/dev/null || \
	 echo "Could not build serial_blas (no BLAS available)"

serial_block: serial_block.c
	$(CC) $(BASE_CFLAGS) $(OPT_FLAGS) -o serial_block serial_block.c -lm

clean:
	rm -f serial_O3 serial_O0 serial_ijk serial_unroll serial_block serial_simd serial_omp serial_blas

.PHONY: run_blas
run_blas: serial_blas
	@echo "Running serial_blas (MKL) for 1024x1024x1024 with LD_PRELOAD and MKL libs"
	@LD_PRELOAD=/lib/x86_64-linux-gnu/libgomp.so.1 LD_LIBRARY_PATH=/opt/intel/oneapi/mkl/2025.3/lib/:$$LD_LIBRARY_PATH ./serial_blas 1024 1024 1024

.PHONY: generate_all
generate_all: serial_O0 serial_O3 serial_blas
	@echo "Generating all CSV results: compare C versions, run MKL, build CSVs"
	@./compare_c_versions.sh
	@$(MAKE) run_blas
	@python3 generate_csvs.py

.PHONY: run_c_direct
run_c_direct: serial_O0 serial_O3
	@echo "Running direct C_O0/C_O3 benchmarks and producing CSVs"
	@python3 run_c_bench.py

.PHONY: generate_and_cleanup
generate_and_cleanup:
	@echo "Running generate_and_cleanup.py to build and produce CSVs"
	@python3 generate_and_cleanup.py
