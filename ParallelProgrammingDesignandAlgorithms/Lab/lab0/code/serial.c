#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double now_seconds(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

static double rand_double(void) {
    return (double)rand() / (double)RAND_MAX;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s m n k\n", argv[0]);
        return 1;
    }
    int m, n, k;
    if (argc == 2) {
        int N = atoi(argv[1]); m = n = k = N;
    } else {
        m = atoi(argv[1]); n = atoi(argv[2]); k = atoi(argv[3]);
    }
    srand(0);
    double *A = malloc(sizeof(double) * (size_t)m * k);
    double *B = malloc(sizeof(double) * (size_t)k * n);
    double *C = calloc((size_t)m * n, sizeof(double));
    if (!A || !B || !C) {
        perror("alloc");
        return 1;
    }

    for (size_t i = 0; i < (size_t)m * k; ++i) A[i] = rand_double();
    for (size_t i = 0; i < (size_t)k * n; ++i) B[i] = rand_double();

    double t0 = now_seconds();
    for (int i = 0; i < m; ++i) {
        for (int kk = 0; kk < k; ++kk) {
            double aik = A[(size_t)i * k + kk];
            for (int j = 0; j < n; ++j) {
                C[(size_t)i * n + j] += aik * B[(size_t)kk * n + j];
            }
        }
    }
    double t1 = now_seconds();
    double checksum = 0.0;
    for (size_t i = 0; i < (size_t)m * n; ++i) checksum += C[i];
    printf("%d %d %d %f %e\n", m, n, k, t1 - t0, checksum);

    free(A);
    free(B);
    free(C);
    return 0;
}
