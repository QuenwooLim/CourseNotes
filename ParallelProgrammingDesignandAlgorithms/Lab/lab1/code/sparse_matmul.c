#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <mpi.h>

static long get_maxrss_kb(void) {
    struct rusage r;
    getrusage(RUSAGE_SELF, &r);
    return r.ru_maxrss;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int N = 1000;
    double density = 0.01;
    int runs = 1;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--size") == 0 && i+1 < argc) N = atoi(argv[++i]);
        else if (strcmp(argv[i], "--density") == 0 && i+1 < argc) density = atof(argv[++i]);
        else if (strcmp(argv[i], "--runs") == 0 && i+1 < argc) runs = atoi(argv[++i]);
    }

    int base = N / nprocs;
    int rem = N % nprocs;
    int *rows = (int*)malloc(nprocs * sizeof(int));
    int *displs = (int*)malloc(nprocs * sizeof(int));
    for (int i = 0; i < nprocs; ++i) {
        rows[i] = base + (i < rem ? 1 : 0);
        displs[i] = (i==0) ? 0 : displs[i-1] + rows[i-1];
    }

    int local_rows = rows[rank];

    double *B = (double*)malloc(sizeof(double) * (long)N * N);
    if (!B) { fprintf(stderr, "alloc B failed N=%d\n", N); MPI_Abort(MPI_COMM_WORLD,1); }

    int *row_ptr = NULL;
    double *vals = NULL;
    int *cols = NULL;
    long nnz = 0;

    if (rank == 0) {
        srand(0);
        for (long i = 0; i < (long)N * N; ++i) B[i] = (double)rand() / RAND_MAX;

        row_ptr = calloc(N+1, sizeof(int));
        if (!row_ptr) { MPI_Abort(MPI_COMM_WORLD,1); }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                double r = (double)rand() / RAND_MAX;
                if (r < density) { row_ptr[i+1]++; nnz++; }
            }
        }
        for (int i = 1; i <= N; ++i) row_ptr[i] += row_ptr[i-1];

        vals = malloc(sizeof(double) * nnz);
        cols = malloc(sizeof(int) * nnz);
        if (!vals || !cols) { MPI_Abort(MPI_COMM_WORLD,1); }

        long idx = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                double r = (double)rand() / RAND_MAX;
                if (r < density) {
                    vals[idx] = (double)rand() / RAND_MAX;
                    cols[idx] = j;
                    idx++;
                }
            }
        }
    }

    int *sendcounts_rowptr = (int*)malloc(nprocs * sizeof(int));
    int *senddispls_rowptr = (int*)malloc(nprocs * sizeof(int));
    int *sendcounts_vals = (int*)malloc(nprocs * sizeof(int));
    int *senddispls_vals = (int*)malloc(nprocs * sizeof(int));
    for (int i = 0; i < nprocs; ++i) {
        sendcounts_rowptr[i] = rows[i] + 1;
        senddispls_rowptr[i] = displs[i];
        if (rank == 0) {
            int start = displs[i];
            int end = displs[i] + rows[i];
            int cnt = row_ptr[end] - row_ptr[start];
            sendcounts_vals[i] = cnt;
            senddispls_vals[i] = (start < N) ? row_ptr[start] : 0;
        } else {
            sendcounts_vals[i] = 0;
            senddispls_vals[i] = 0;
        }
    }

    int local_nnz = 0;
    for (int i = 0; i < nprocs; ++i) if (i == rank) local_nnz = sendcounts_vals[i];

    int *local_row_ptr = (int*)malloc((local_rows + 1) * sizeof(int));
    double *local_vals = NULL;
    int *local_cols = NULL;
    if (local_nnz > 0) {
        local_vals = (double*)malloc(sizeof(double) * (size_t)local_nnz);
        local_cols = (int*)malloc(sizeof(int) * (size_t)local_nnz);
    }

    MPI_Scatterv(row_ptr, sendcounts_rowptr, senddispls_rowptr, MPI_INT,
                 local_row_ptr, local_rows + 1, MPI_INT,
                 0, MPI_COMM_WORLD);

    MPI_Scatterv(vals, sendcounts_vals, senddispls_vals, MPI_DOUBLE,
                 local_vals, local_nnz, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    MPI_Scatterv(cols, sendcounts_vals, senddispls_vals, MPI_INT,
                 local_cols, local_nnz, MPI_INT,
                 0, MPI_COMM_WORLD);

    int shift = local_row_ptr[0];
    for (int i = 0; i < local_rows + 1; ++i) local_row_ptr[i] -= shift;

    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double tsum = 0.0;
    for (int run = 0; run < runs; ++run) {
        MPI_Barrier(MPI_COMM_WORLD);
        double t0 = MPI_Wtime();

        double *Crow = (double*)malloc(sizeof(double) * N);
        for (int ii = 0; ii < local_rows; ++ii) {
            memset(Crow, 0, sizeof(double) * N);
            for (int p = local_row_ptr[ii]; p < local_row_ptr[ii+1]; ++p) {
                int col = local_cols[p];
                double v = local_vals[p];
                double *Brow = &B[(long)col * N];
                for (int j = 0; j < N; ++j) Crow[j] += v * Brow[j];
            }
        }
        free(Crow);

        MPI_Barrier(MPI_COMM_WORLD);
        double t1 = MPI_Wtime();
        tsum += (t1 - t0);
    }

    double avg_local = tsum / runs;
    double avg_max = 0.0;
    MPI_Reduce(&avg_local, &avg_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    long rss_kb = get_maxrss_kb();
    long rss_kb_max = 0;
    MPI_Reduce(&rss_kb, &rss_kb_max, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double rss_mb = (double)rss_kb_max / 1024.0;
        printf("RESULT_SPARSE,N=%d,density=%.6f,avg_time_s=%.6f,max_rss_mb=%.2f\n", N, density, avg_max, rss_mb);
    }

    free(rows); free(displs); free(sendcounts_rowptr); free(senddispls_rowptr);
    free(sendcounts_vals); free(senddispls_vals);
    free(local_row_ptr); if (local_vals) free(local_vals); if (local_cols) free(local_cols);
    if (rank == 0) { free(B); free(row_ptr); if (vals) free(vals); if (cols) free(cols); }
    else free(B);

    MPI_Finalize();
    return 0;
}
