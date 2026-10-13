#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s M N K [-s default|static|dynamic] [-q]\n", argv[0]);
        fprintf(stderr, "  M: rows of A, N: cols of A / rows of B, K: cols of B\n");
        fprintf(stderr, "  -s: schedule type (default: default)\n");
        fprintf(stderr, "  -q: quiet mode, suppress matrix output\n");
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int K = atoi(argv[3]);

    const char *schedule_type = "default";
    int quiet = 0;

    for (int i = 4; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            schedule_type = argv[++i];
        } else if (strcmp(argv[i], "-q") == 0) {
            quiet = 1;
        }
    }

    double *A = (double *)malloc(M * N * sizeof(double));
    double *B = (double *)malloc(N * K * sizeof(double));
    double *C = (double *)malloc(M * K * sizeof(double));

    srand(42);
    for (int i = 0; i < M * N; i++) A[i] = (double)rand() / RAND_MAX;
    for (int i = 0; i < N * K; i++) B[i] = (double)rand() / RAND_MAX;
    for (int i = 0; i < M * K; i++) C[i] = 0.0;

    double start = omp_get_wtime();

    if (strcmp(schedule_type, "static") == 0) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < M; i++) {
            for (int k = 0; k < N; k++) {
                double aik = A[i * N + k];
                for (int j = 0; j < K; j++) {
                    C[i * K + j] += aik * B[k * K + j];
                }
            }
        }
    } else if (strcmp(schedule_type, "dynamic") == 0) {
        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < M; i++) {
            for (int k = 0; k < N; k++) {
                double aik = A[i * N + k];
                for (int j = 0; j < K; j++) {
                    C[i * K + j] += aik * B[k * K + j];
                }
            }
        }
    } else {
        #pragma omp parallel for
        for (int i = 0; i < M; i++) {
            for (int k = 0; k < N; k++) {
                double aik = A[i * N + k];
                for (int j = 0; j < K; j++) {
                    C[i * K + j] += aik * B[k * K + j];
                }
            }
        }
    }

    double elapsed = omp_get_wtime() - start;

    if (!quiet) {
        printf("Matrix A (%dx%d):\n", M, N);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) printf("%.4f ", A[i * N + j]);
            printf("\n");
        }
        printf("\nMatrix B (%dx%d):\n", N, K);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < K; j++) printf("%.4f ", B[i * K + j]);
            printf("\n");
        }
        printf("\nMatrix C (%dx%d):\n", M, K);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < K; j++) printf("%.4f ", C[i * K + j]);
            printf("\n");
        }
    }

    printf("ELAPSED: %.6f\n", elapsed);

    free(A);
    free(B);
    free(C);
    return 0;
}