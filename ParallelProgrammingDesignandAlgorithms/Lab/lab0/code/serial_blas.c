#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef USE_BLAS
#include <cblas.h>
#endif

static double now_seconds(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s m n k\n", argv[0]);
        return 1;
    }
    int m, n, k;
    if (argc == 2) { int N = atoi(argv[1]); m = n = k = N; } else { m = atoi(argv[1]); n = atoi(argv[2]); k = atoi(argv[3]); }
    double *A = malloc(sizeof(double) * (size_t)m * k);
    double *B = malloc(sizeof(double) * (size_t)k * n);
    double *C = malloc(sizeof(double) * (size_t)m * n);
    if (!A || !B || !C) { perror("alloc"); return 1; }
    for (size_t i = 0; i < (size_t)m * k; ++i) A[i] = rand() / (double)RAND_MAX;
    for (size_t i = 0; i < (size_t)k * n; ++i) B[i] = rand() / (double)RAND_MAX;

#ifdef USE_BLAS
    double alpha = 1.0, beta = 0.0;
    double t0 = now_seconds();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, alpha, A, k, B, n, beta, C, n);
    double t1 = now_seconds();
    double checksum = 0.0; for (size_t i = 0; i < (size_t)m * n; ++i) checksum += C[i];
    printf("%d %d %d %f %e\n", m, n, k, t1 - t0, checksum);
#else
    fprintf(stderr, "BLAS not enabled at compile time\n");
    return 2;
#endif
    free(A); free(B); free(C);
    return 0;
}
