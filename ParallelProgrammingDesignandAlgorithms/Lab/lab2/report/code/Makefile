CC=mpicc
CFLAGS=-O3 -std=c99 -Wall -Wextra
TARGET=parallel_matmul_mpi_v2
BIN_DIR=bin

all: $(TARGET)

$(TARGET): parallel_matmul_mpi_v2.c
	$(CC) $(CFLAGS) -o $(TARGET) parallel_matmul_mpi_v2.c

.PHONY: install-bin
install-bin:
	mkdir -p $(BIN_DIR)
	@if [ -f parallel_matmul_p2p.c ]; then $(CC) $(CFLAGS) -o $(BIN_DIR)/parallel_matmul_p2p parallel_matmul_p2p.c; fi

clean:
	rm -f $(TARGET) $(BIN_DIR)/* *.bin || true
