MPICC=mpicc
CC=gcc
CFLAGS_MPI=-O3 -std=c99 -Wall -Wextra
CFLAGS_PTHREAD=-O2 -std=c11 -Wall -Wextra -pthread

MPI_TARGET=parallel_matmul_mpi_v2
PTHREAD_TARGETS=task1_quadratic task2_mcPI
BIN_DIR=bin

all: $(PTHREAD_TARGETS)

$(MPI_TARGET): main.c
	$(MPICC) $(CFLAGS_MPI) -o $(MPI_TARGET) main.c


task1_quadratic: task1_quadratic.c
	$(CC) $(CFLAGS_PTHREAD) -O2 -march=native -o task1_quadratic task1_quadratic.c -lm

task2_mcPI: task2_mcPI.c
	$(CC) $(CFLAGS_PTHREAD) -O2 -march=native -o task2_mcPI task2_mcPI.c -lm

.PHONY: install-bin
install-bin:
	mkdir -p $(BIN_DIR)
	@if [ -f main.c ]; then $(MPICC) $(CFLAGS_MPI) -o $(BIN_DIR)/$(MPI_TARGET) main.c; fi
	@if [ -f task1_quadratic.c ]; then $(CC) $(CFLAGS_PTHREAD) -o $(BIN_DIR)/task1_quadratic task1_quadratic.c -lm; fi
	@if [ -f task2_mcPI.c ]; then $(CC) $(CFLAGS_PTHREAD) -o $(BIN_DIR)/task2_mcPI task2_mcPI.c -lm; fi

clean:
	rm -f $(MPI_TARGET) $(PTHREAD_TARGETS) $(BIN_DIR)/* *.bin results_*.csv results_quadratic.csv results_pi.csv || true
