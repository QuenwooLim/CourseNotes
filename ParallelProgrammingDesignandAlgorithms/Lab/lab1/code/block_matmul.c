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

static void fill_rand(double *M, int N) {
    for (long i = 0; i < (long)N * N; ++i) M[i] = (double)rand() / RAND_MAX;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int N = 512;
    int Bsize = 64;
    int runs = 1;
    char mode[32] = "compare";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--size") == 0 && i+1 < argc) N = atoi(argv[++i]);
        else if (strcmp(argv[i], "--block") == 0 && i+1 < argc) Bsize = atoi(argv[++i]);
        else if (strcmp(argv[i], "--runs") == 0 && i+1 < argc) runs = atoi(argv[++i]);
        else if (strcmp(argv[i], "--mode") == 0 && i+1 < argc) strncpy(mode, argv[++i], sizeof(mode)-1);
    }

    srand(0 + rank);

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
        if (!A || !B || !C) {
            fprintf(stderr, "Allocation failed for N=%d\n", N);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fill_rand(A, N); fill_rand(B, N);
    } else {
        if (!B || !local_A || !local_C) {
            fprintf(stderr, "Allocation failed on rank %d for N=%d\n", rank, N);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int *sendcounts = (int*)malloc(nprocs * sizeof(int));
    int *senddispls = (int*)malloc(nprocs * sizeof(int));
    for (int i = 0; i < nprocs; ++i) {
        sendcounts[i] = rows[i] * N;
        senddispls[i] = displs[i] * N;
    }

    MPI_Scatterv(A, sendcounts, senddispls, MPI_DOUBLE,
                 local_A, local_rows * N, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (strcmp(mode, "naive") == 0) {
        double tsum = 0.0;
        for (int r = 0; r < runs; ++r) {
            MPI_Barrier(MPI_COMM_WORLD);
            double t0 = MPI_Wtime();

            for (int i = 0; i < local_rows; ++i) {
                for (int j = 0; j < N; ++j) {
                    double s = 0.0;
                    for (int k = 0; k < N; ++k) s += local_A[(long)i*N + k] * B[(long)k*N + j];
                    local_C[(long)i*N + j] = s;
                }
            }

            MPI_Barrier(MPI_COMM_WORLD);
            double t1 = MPI_Wtime();
            tsum += (t1 - t0);
        }
        double avg_local = tsum / runs;
        double avg_max;
        MPI_Reduce(&avg_local, &avg_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        long rss_kb = get_maxrss_kb();
        long rss_kb_max = 0;
        MPI_Reduce(&rss_kb, &rss_kb_max, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            double rss_mb = (double)rss_kb_max / 1024.0;
            printf("RESULT,N=%d,block=%d,mode=naive,avg_time_s=%.6f,max_rss_mb=%.2f\n", N, Bsize, avg_max, rss_mb);
        }
    } else if (strcmp(mode, "blocked") == 0) {
        double tsum = 0.0;
        for (int r = 0; r < runs; ++r) {
            MPI_Barrier(MPI_COMM_WORLD);
            double t0 = MPI_Wtime();

            memset(local_C, 0, sizeof(double) * (long)local_rows * N);
            for (int ii = 0; ii < local_rows; ii += Bsize)
                for (int kk = 0; kk < N; kk += Bsize)
                    for (int jj = 0; jj < N; jj += Bsize) {
                        int iimax = ii + Bsize < local_rows ? ii + Bsize : local_rows;
                        int kkmax = kk + Bsize < N ? kk + Bsize : N;
                        int jjmax = jj + Bsize < N ? jj + Bsize : N;
                        for (int i = ii; i < iimax; ++i)
                            for (int k = kk; k < kkmax; ++k) {
                                double aik = local_A[(long)i*N + k];
                                for (int j = jj; j < jjmax; ++j)
                                    local_C[(long)i*N + j] += aik * B[(long)k*N + j];
                            }
                    }

            MPI_Barrier(MPI_COMM_WORLD);
            double t1 = MPI_Wtime();
            tsum += (t1 - t0);
        }
        double avg_local = tsum / runs;
        double avg_max;
        MPI_Reduce(&avg_local, &avg_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        long rss_kb = get_maxrss_kb();
        long rss_kb_max = 0;
        MPI_Reduce(&rss_kb, &rss_kb_max, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            double rss_mb = (double)rss_kb_max / 1024.0;
            printf("RESULT,N=%d,block=%d,mode=blocked,avg_time_s=%.6f,max_rss_mb=%.2f\n", N, Bsize, avg_max, rss_mb);
        }
    } else {
        double tsum_n = 0.0;
        for (int r = 0; r < runs; ++r) {
            MPI_Barrier(MPI_COMM_WORLD);
            double t0 = MPI_Wtime();

            for (int i = 0; i < local_rows; ++i) {
                for (int j = 0; j < N; ++j) {
                    double s = 0.0;
                    for (int k = 0; k < N; ++k) s += local_A[(long)i*N + k] * B[(long)k*N + j];
                    local_C[(long)i*N + j] = s;
                }
            }

            MPI_Barrier(MPI_COMM_WORLD);
            double t1 = MPI_Wtime();
            tsum_n += (t1 - t0);
        }
        double avgn_local = tsum_n / runs;
        double avgn_max;
        MPI_Reduce(&avgn_local, &avgn_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        long rssn_kb = get_maxrss_kb();
        long rssn_kb_max = 0;
        MPI_Reduce(&rssn_kb, &rssn_kb_max, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);

        double tsum_b = 0.0;
        for (int r = 0; r < runs; ++r) {
            MPI_Barrier(MPI_COMM_WORLD);
            double t0 = MPI_Wtime();

            memset(local_C, 0, sizeof(double) * (long)local_rows * N);
            for (int ii = 0; ii < local_rows; ii += Bsize)
                for (int kk = 0; kk < N; kk += Bsize)
                    for (int jj = 0; jj < N; jj += Bsize) {
                        int iimax = ii + Bsize < local_rows ? ii + Bsize : local_rows;
                        int kkmax = kk + Bsize < N ? kk + Bsize : N;
                        int jjmax = jj + Bsize < N ? jj + Bsize : N;
                        for (int i = ii; i < iimax; ++i)
                            for (int k = kk; k < kkmax; ++k) {
                                double aik = local_A[(long)i*N + k];
                                for (int j = jj; j < jjmax; ++j)
                                    local_C[(long)i*N + j] += aik * B[(long)k*N + j];
                            }
                    }

            MPI_Barrier(MPI_COMM_WORLD);
            double t1 = MPI_Wtime();
            tsum_b += (t1 - t0);
        }
        double avgb_local = tsum_b / runs;
        double avgb_max;
        MPI_Reduce(&avgb_local, &avgb_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        long rssb_kb = get_maxrss_kb();
        long rssb_kb_max = 0;
        MPI_Reduce(&rssb_kb, &rssb_kb_max, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            double rssn_mb = (double)rssn_kb_max / 1024.0;
            double rssb_mb = (double)rssb_kb_max / 1024.0;
            printf("COMPARE,N=%d,block=%d,naive_time_s=%.6f,naive_rss_mb=%.2f,blocked_time_s=%.6f,blocked_rss_mb=%.2f\n",
                   N, Bsize, avgn_max, rssn_mb, avgb_max, rssb_mb);
        }
    }

    free(rows); free(displs); free(sendcounts); free(senddispls);
    free(local_A); free(local_C); free(B);
    if (rank == 0) { free(A); free(C); }

    MPI_Finalize();
    return 0;
}
