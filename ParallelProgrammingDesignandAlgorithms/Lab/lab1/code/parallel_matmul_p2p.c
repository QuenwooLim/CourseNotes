#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_arg_int(int argc, char** argv, const char* key, int default_v) {
    for (int i = 1; i < argc-1; ++i) {
        if (strcmp(argv[i], key) == 0) return atoi(argv[i+1]);
    }
    return default_v;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int N = parse_arg_int(argc, argv, "--size", 512);
    int runs = parse_arg_int(argc, argv, "--runs", 3);

    int base = N / nprocs;
    int rem = N % nprocs;
    int *rows = (int*)malloc(nprocs * sizeof(int));
    int *displs = (int*)malloc(nprocs * sizeof(int));
    for (int i = 0; i < nprocs; ++i) {
        rows[i] = base + (i < rem ? 1 : 0);
        displs[i] = (i==0) ? 0 : displs[i-1] + rows[i-1];
    }

    int local_rows = rows[rank];

    double *local_A = (double*)malloc((size_t)local_rows * N * sizeof(double));
    double *B = (double*)malloc((size_t)N * N * sizeof(double));
    double *local_C = (double*)malloc((size_t)local_rows * N * sizeof(double));

    double *A = NULL;
    double *C = NULL;

    if (rank == 0) {
        A = (double*)malloc((size_t)N * N * sizeof(double));
        C = (double*)malloc((size_t)N * N * sizeof(double));
        srand(42);
        for (int i = 0; i < N*N; ++i) A[i] = ((double)rand())/RAND_MAX;
        for (int i = 0; i < N*N; ++i) B[i] = ((double)rand())/RAND_MAX;

        for (int p = 1; p < nprocs; ++p) {
            int cnt = rows[p] * N;
            MPI_Send(A + displs[p]*N, cnt, MPI_DOUBLE, p, 10, MPI_COMM_WORLD);
            MPI_Send(B, N*N, MPI_DOUBLE, p, 11, MPI_COMM_WORLD);
        }

        memcpy(local_A, A + displs[0]*N, (size_t)local_rows * N * sizeof(double));
    } else {

        MPI_Recv(local_A, local_rows * N, MPI_DOUBLE, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(B, N*N, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    double local_time = 0.0;
    for (int r = 0; r < runs; ++r) {
        MPI_Barrier(MPI_COMM_WORLD);
        double t0 = MPI_Wtime();

        for (int i = 0; i < local_rows; ++i) {
            for (int j = 0; j < N; ++j) {
                double s = 0.0;
                for (int k = 0; k < N; ++k) {
                    s += local_A[i*N + k] * B[k*N + j];
                }
                local_C[i*N + j] = s;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double t1 = MPI_Wtime();
        local_time += (t1 - t0);
    }
    local_time /= runs;

    if (rank == 0) {
        memcpy(C + displs[0]*N, local_C, (size_t)local_rows * N * sizeof(double));
        for (int p = 1; p < nprocs; ++p) {
            MPI_Recv(C + displs[p]*N, rows[p] * N, MPI_DOUBLE, p, 12, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        double maxt = local_time;
        for (int p = 1; p < nprocs; ++p) {
            double trecv;
            MPI_Recv(&trecv, 1, MPI_DOUBLE, p, 13, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (trecv > maxt) maxt = trecv;
        }
        printf("N=%d, procs=%d, runs=%d, approx_time(s)=%.6f\n", N, nprocs, runs, maxt);
    } else {
        MPI_Send(local_C, local_rows * N, MPI_DOUBLE, 0, 12, MPI_COMM_WORLD);
        MPI_Send(&local_time, 1, MPI_DOUBLE, 0, 13, MPI_COMM_WORLD);
    }

    free(rows); free(displs);
    free(local_A); free(local_C); free(B);
    if (rank == 0) { free(A); free(C); }

    MPI_Finalize();
    return 0;
}
