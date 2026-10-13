
CXX = mpicc
CFLAGS = -O2 -std=c99 -Wall
BIN_DIR = bin

all: $(BIN_DIR) $(BIN_DIR)/parallel_matmul_p2p $(BIN_DIR)/block_matmul $(BIN_DIR)/sparse_matmul
CFLAGS = -O2 -std=c99 -Wall
BIN_DIR = bin

all: $(BIN_DIR) $(BIN_DIR)/parallel_matmul_p2p

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/parallel_matmul_p2p: parallel_matmul_p2p.c | $(BIN_DIR)
	$(CXX) $(CFLAGS) -o $(BIN_DIR)/parallel_matmul_p2p parallel_matmul_p2p.c

$(BIN_DIR)/block_matmul: block_matmul.c | $(BIN_DIR)
	$(CXX) $(CFLAGS) -o $(BIN_DIR)/block_matmul block_matmul.c -lm

$(BIN_DIR)/sparse_matmul: sparse_matmul.c | $(BIN_DIR)
	$(CXX) $(CFLAGS) -o $(BIN_DIR)/sparse_matmul sparse_matmul.c -lm

$(BIN_DIR)/mpi_hello: mpi_ping_pong/mpi_hello.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/mpi_hello mpi_ping_pong/mpi_hello.c

$(BIN_DIR)/mpi_ping_pong: mpi_ping_pong/mpi_ping_pong.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/mpi_ping_pong mpi_ping_pong/mpi_ping_pong.c

clean:
	rm -rf $(BIN_DIR)
