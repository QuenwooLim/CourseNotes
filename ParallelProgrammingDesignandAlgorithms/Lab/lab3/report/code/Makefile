MPICC=mpicc
CC=gcc
CFLAGS_MPI=-O3 -std=c99 -Wall -Wextra
CFLAGS_PTHREAD=-O2 -std=c11 -Wall -Wextra -pthread

MPI_TARGET=parallel_matmul_mpi_v2
PTHREAD_TARGETS=matmul_pthreads sum_pthreads
BIN_DIR=bin

all: $(MPI_TARGET) $(PTHREAD_TARGETS)

$(MPI_TARGET): main.c
	$(MPICC) $(CFLAGS_MPI) -o $(MPI_TARGET) main.c

matmul_pthreads: matmul_pthreads.c
	$(CC) $(CFLAGS_PTHREAD) -O2 -march=native -o matmul_pthreads matmul_pthreads.c

sum_pthreads: sum_pthreads.c
	$(CC) $(CFLAGS_PTHREAD) -O2 -march=native -o sum_pthreads sum_pthreads.c

.PHONY: install-bin
install-bin:
	mkdir -p $(BIN_DIR)
	@if [ -f main.c ]; then $(MPICC) $(CFLAGS_MPI) -o $(BIN_DIR)/$(MPI_TARGET) main.c; fi
	@if [ -f matmul_pthreads.c ]; then $(CC) $(CFLAGS_PTHREAD) -o $(BIN_DIR)/matmul_pthreads matmul_pthreads.c; fi
	@if [ -f sum_pthreads.c ]; then $(CC) $(CFLAGS_PTHREAD) -o $(BIN_DIR)/sum_pthreads sum_pthreads.c; fi

clean:
	rm -f $(MPI_TARGET) $(PTHREAD_TARGETS) $(BIN_DIR)/* *.bin results_*.csv || true
