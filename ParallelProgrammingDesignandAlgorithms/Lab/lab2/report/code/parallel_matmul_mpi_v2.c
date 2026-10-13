/*
MPI 并行矩阵乘法 v2
    - 通信方式
        - p2p 
        - collective
    - 划分方式
        - block 
        - cyclic
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

int parse_arg_int(int argc, char** argv, const char* key, int default_v) {
    for (int i = 1; i < argc-1; ++i) {
        if (strcmp(argv[i], key) == 0) return atoi(argv[i+1]);
    }
    return default_v;
}

const char* parse_arg_str(int argc, char** argv, const char* key, const char* default_v) {
    for (int i = 1; i < argc-1; ++i) {
        if (strcmp(argv[i], key) == 0) return argv[i+1];
    }
    return default_v;
}

static void write_matrix_bin(const char* path, double* M, int r, int c) {
    FILE* f = fopen(path, "wb");
    if (!f) { perror("fopen"); return; }
    fwrite(&r, sizeof(int), 1, f);
    fwrite(&c, sizeof(int), 1, f);
    fwrite(M, sizeof(double), (size_t)r * c, f);
    fclose(f);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int m = parse_arg_int(argc, argv, "--m", 512);
    int n = parse_arg_int(argc, argv, "--n", 512);
    int k = parse_arg_int(argc, argv, "--k", 512);
    int runs = parse_arg_int(argc, argv, "--runs", 3);
    const char* method = parse_arg_str(argc, argv, "--method", "coll"); // p2p or coll
    const char* partition = parse_arg_str(argc, argv, "--partition", "block"); // block or cyclic
    int save = parse_arg_int(argc, argv, "--save", 0);
    int seed = parse_arg_int(argc, argv, "--seed", 42);

    if (rank == 0) {
        printf("MPI-matmul v2: m=%d n=%d k=%d procs=%d method=%s partition=%s runs=%d\n",
               m, n, k, nprocs, method, partition, runs);
    }

    // compute rows distribution (counts in number of rows)
    int base = m / nprocs;
    int rem = m % nprocs;
    int *rows = (int*)malloc(nprocs * sizeof(int));
    int *row_displ = (int*)malloc(nprocs * sizeof(int));
    for (int i = 0; i < nprocs; ++i) {
        rows[i] = base + (i < rem ? 1 : 0);
        row_displ[i] = (i==0) ? 0 : row_displ[i-1] + rows[i-1];
    }

    // aggregate per-process metadata into a struct and broadcast
    typedef struct { int rows; int row_off; } ProcMeta;
    ProcMeta *metas = (ProcMeta*)malloc(nprocs * sizeof(ProcMeta));
    if (rank == 0) {
        for (int p = 0; p < nprocs; ++p) { metas[p].rows = rows[p]; metas[p].row_off = row_displ[p]; }
    }

    MPI_Datatype meta_type;
    int blocklens[2] = {1,1};
    MPI_Aint offsets[2];
    offsets[0] = offsetof(ProcMeta, rows);
    offsets[1] = offsetof(ProcMeta, row_off);
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Type_create_struct(2, blocklens, offsets, types, &meta_type);
    MPI_Type_commit(&meta_type);

    MPI_Bcast(metas, nprocs, meta_type, 0, MPI_COMM_WORLD);

    int local_rows = metas[rank].rows;

    // allocate local buffers
    double *local_A = (double*)malloc((size_t)local_rows * n * sizeof(double));
    double *B = (double*)malloc((size_t)n * k * sizeof(double));
    double *local_C = (double*)malloc((size_t)local_rows * k * sizeof(double));

    double *A = NULL;
    double *C = NULL;

    if (rank == 0) {
        A = (double*)malloc((size_t)m * n * sizeof(double));
        C = (double*)malloc((size_t)m * k * sizeof(double));
        srand(seed);
        for (long i = 0; i < (long)m * n; ++i) A[i] = ((double)rand())/RAND_MAX;
        for (long i = 0; i < (long)n * k; ++i) B[i] = ((double)rand())/RAND_MAX;
    }

    // communication: two methods (p2p / coll) and two partitions (block / cyclic)
    if (strcmp(method, "p2p") == 0) {
        // point-to-point distribution
        if (strcmp(partition, "block") == 0) {
            if (rank == 0) {
                for (int p = 1; p < nprocs; ++p) {
                    int cnt = rows[p] * n;
                    if (cnt > 0) MPI_Send(A + (size_t)row_displ[p] * n, cnt, MPI_DOUBLE, p, 10, MPI_COMM_WORLD);
                    MPI_Send(B, n * k, MPI_DOUBLE, p, 11, MPI_COMM_WORLD);
                }
                // copy local block
                if (local_rows > 0) memcpy(local_A, A + (size_t)row_displ[0] * n, (size_t)local_rows * n * sizeof(double));
            } else {
                if (local_rows > 0) MPI_Recv(local_A, local_rows * n, MPI_DOUBLE, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(B, n * k, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else { // cyclic p2p: pack on root and send
            int *sendcounts = (int*)malloc(nprocs * sizeof(int));
            int *senddispl = (int*)malloc(nprocs * sizeof(int));
            for (int p = 0; p < nprocs; ++p) sendcounts[p] = rows[p] * n;

            if (rank == 0) {
                double *packed = (double*)malloc((size_t)m * n * sizeof(double));
                int off = 0;
                for (int p = 0; p < nprocs; ++p) {
                    senddispl[p] = off;
                    for (int r = 0; r < m; ++r) if ((r % nprocs) == p) {
                        memcpy(packed + off, A + (size_t)r * n, n * sizeof(double));
                        off += n;
                    }
                }
                for (int p = 1; p < nprocs; ++p) {
                    if (sendcounts[p] > 0) MPI_Send(packed + senddispl[p], sendcounts[p], MPI_DOUBLE, p, 20, MPI_COMM_WORLD);
                    MPI_Send(B, n * k, MPI_DOUBLE, p, 21, MPI_COMM_WORLD);
                }
                if (sendcounts[0] > 0) memcpy(local_A, packed + senddispl[0], (size_t)sendcounts[0] * sizeof(double));
                free(packed);
            } else {
                if (local_rows > 0) MPI_Recv(local_A, local_rows * n, MPI_DOUBLE, 0, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(B, n * k, MPI_DOUBLE, 0, 21, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            free(sendcounts); free(senddispl);
        }

        // compute local product
        double local_time = 0.0;
        for (int r = 0; r < runs; ++r) {
            MPI_Barrier(MPI_COMM_WORLD);
            double t0 = MPI_Wtime();
            for (int i = 0; i < local_rows; ++i) {
                for (int j = 0; j < k; ++j) {
                    double s = 0.0;
                    for (int t = 0; t < n; ++t) s += local_A[(size_t)i * n + t] * B[(size_t)t * k + j];
                    local_C[(size_t)i * k + j] = s;
                }
            }
            MPI_Barrier(MPI_COMM_WORLD);
            double t1 = MPI_Wtime();
            local_time += (t1 - t0);
        }
        local_time /= runs;

        // gather results back to root
        if (strcmp(partition, "block") == 0) {
            if (rank == 0) {
                if (local_rows > 0) memcpy(C + (size_t)row_displ[0] * k, local_C, (size_t)local_rows * k * sizeof(double));
                double maxt = local_time;
                for (int p = 1; p < nprocs; ++p) {
                    if (rows[p] > 0) MPI_Recv(C + (size_t)row_displ[p] * k, rows[p] * k, MPI_DOUBLE, p, 12, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    double trecv; MPI_Recv(&trecv, 1, MPI_DOUBLE, p, 13, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    if (trecv > maxt) maxt = trecv;
                }
                printf("method=p2p partition=block m=%d n=%d k=%d procs=%d runs=%d time(s)=%.6f\n", m, n, k, nprocs, runs, maxt);
            } else {
                if (local_rows > 0) MPI_Send(local_C, local_rows * k, MPI_DOUBLE, 0, 12, MPI_COMM_WORLD);
                MPI_Send(&local_time, 1, MPI_DOUBLE, 0, 13, MPI_COMM_WORLD);
            }
        } else { // cyclic p2p gather: root reorders
            int *recvcounts = (int*)malloc(nprocs * sizeof(int));
            int *recvdispl = (int*)malloc(nprocs * sizeof(int));
            for (int p = 0; p < nprocs; ++p) recvcounts[p] = rows[p] * k;
            if (rank == 0) {
                double *packedC = (double*)malloc((size_t)m * k * sizeof(double));
                int off = 0;
                for (int p = 0; p < nprocs; ++p) { recvdispl[p] = off; off += recvcounts[p]; }

                if (recvcounts[0] > 0) memcpy(packedC + recvdispl[0], local_C, (size_t)recvcounts[0] * sizeof(double));
                for (int p = 1; p < nprocs; ++p) {
                    if (recvcounts[p] > 0) MPI_Recv(packedC + recvdispl[p], recvcounts[p], MPI_DOUBLE, p, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                // reorder packedC to C (cyclic rows)
                for (int r = 0; r < m; ++r) {
                    int owner = r % nprocs;
                    memcpy(C + (size_t)r * k, packedC + recvdispl[owner] + (size_t)( (r - owner) / nprocs ) * k, k * sizeof(double));
                }

                double maxt = local_time;
                for (int p = 1; p < nprocs; ++p) { double trecv; MPI_Recv(&trecv, 1, MPI_DOUBLE, p, 23, MPI_COMM_WORLD, MPI_STATUS_IGNORE); if (trecv > maxt) maxt = trecv; }
                printf("method=p2p partition=cyclic m=%d n=%d k=%d procs=%d runs=%d time(s)=%.6f\n", m, n, k, nprocs, runs, maxt);
                free(packedC);
            } else {
                if (local_rows > 0) MPI_Send(local_C, local_rows * k, MPI_DOUBLE, 0, 22, MPI_COMM_WORLD);
                MPI_Send(&local_time, 1, MPI_DOUBLE, 0, 23, MPI_COMM_WORLD);
            }
            free(recvcounts); free(recvdispl);
        }

    } else {
        // collective implementation using Scatterv/Bcast/Gatherv
        int *sendcounts = (int*)malloc(nprocs * sizeof(int));
        int *senddispls = (int*)malloc(nprocs * sizeof(int));
        for (int p = 0; p < nprocs; ++p) sendcounts[p] = rows[p] * n;
        for (int p = 0; p < nprocs; ++p) senddispls[p] = row_displ[p] * n;

        if (strcmp(partition, "block") == 0) {
            // A is laid out row-major on root, we can Scatterv directly
            MPI_Scatterv(A, sendcounts, senddispls, MPI_DOUBLE, local_A, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        } else {
            // cyclic: pack on root to make per-process contiguous send blocks
            if (rank == 0) {
                double *packed = (double*)malloc((size_t)m * n * sizeof(double));
                int off = 0;
                for (int p = 0; p < nprocs; ++p) {
                    senddispls[p] = off;
                    for (int r = 0; r < m; ++r) if ((r % nprocs) == p) {
                        memcpy(packed + off, A + (size_t)r * n, n * sizeof(double));
                        off += n;
                    }
                }
                MPI_Scatterv(packed, sendcounts, senddispls, MPI_DOUBLE, local_A, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
                free(packed);
            } else {
                // non-root just call Scatterv to receive
                MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_A, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            }
        }

        // broadcast B to all processes
        MPI_Bcast(B, n * k, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // local multiply and timed runs
        double local_time = 0.0;
        for (int r = 0; r < runs; ++r) {
            MPI_Barrier(MPI_COMM_WORLD);
            double t0 = MPI_Wtime();
            for (int i = 0; i < local_rows; ++i) {
                for (int j = 0; j < k; ++j) {
                    double s = 0.0;
                    for (int t = 0; t < n; ++t) s += local_A[(size_t)i * n + t] * B[(size_t)t * k + j];
                    local_C[(size_t)i * k + j] = s;
                }
            }
            MPI_Barrier(MPI_COMM_WORLD);
            double t1 = MPI_Wtime();
            local_time += (t1 - t0);
        }
        local_time /= runs;

        // gather results to root
        int *recvcounts = (int*)malloc(nprocs * sizeof(int));
        int *recvdispls = (int*)malloc(nprocs * sizeof(int));
        for (int p = 0; p < nprocs; ++p) recvcounts[p] = rows[p] * k;
        for (int p = 0; p < nprocs; ++p) recvdispls[p] = row_displ[p] * k;

        if (strcmp(partition, "block") == 0) {
            MPI_Gatherv(local_C, local_rows * k, MPI_DOUBLE, C, recvcounts, recvdispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        } else {
            // cyclic: gather into packedC then reorder
            if (rank == 0) {
                double *packedC = (double*)malloc((size_t)m * k * sizeof(double));
                // gather into contiguous per-process chunks
                MPI_Gatherv(local_C, local_rows * k, MPI_DOUBLE, packedC, recvcounts, recvdispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);
                // reorder packedC to C
                for (int p = 0; p < nprocs; ++p) {
                    int start = recvdispls[p];
                    int idx = 0;
                    for (int r = 0; r < m; ++r) if ((r % nprocs) == p) {
                        memcpy(C + (size_t)r * k, packedC + (size_t)start + (size_t)idx * k, (size_t)k * sizeof(double));
                        idx++;
                    }
                }
                free(packedC);
            } else {
                MPI_Gatherv(local_C, local_rows * k, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            }
        }

        // reduce max time
        double maxt = 0.0;
        MPI_Reduce(&local_time, &maxt, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            printf("method=collective partition=%s m=%d n=%d k=%d procs=%d runs=%d time(s)=%.6f\n",
                   partition, m, n, k, nprocs, runs, maxt);
        }

        free(sendcounts); free(senddispls); free(recvcounts); free(recvdispls);
    }

    // root optionally save matrices
    if (rank == 0 && save) {
        write_matrix_bin("A.bin", A, m, n);
        write_matrix_bin("B.bin", B, n, k);
        write_matrix_bin("C.bin", C, m, k);
        if (rank == 0) printf("Matrices saved to A.bin/B.bin/C.bin\n");
    }

    // cleanup
    free(rows); free(row_displ); free(metas);
    free(local_A); free(local_C); free(B);
    if (rank == 0) { free(A); free(C); }
    MPI_Type_free(&meta_type);
    MPI_Finalize();
    return 0;
}
