CC = gcc
CFLAGS = -O2

.PHONY: all clean

all: omp_matmul pthread_matmul

omp_matmul: omp_matmul.c
	$(CC) -fopenmp $(CFLAGS) $< -o $@

pthread_matmul: pthread_matmul.c
	$(CC) -pthread $(CFLAGS) $< -o $@

clean:
	rm -f omp_matmul pthread_matmul
