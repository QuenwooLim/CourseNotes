#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double now_seconds(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

static double rand_double(void) { return (double)rand() / (double)RAND_MAX; }

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s m n k [block]", argv[0]);
        return 1;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    int B = (argc > 4) ? atoi(argv[4]) : 64;
    srand(0);
    double *A = malloc(sizeof(double) * (size_t)m * k);
    double *Bmat = malloc(sizeof(double) * (size_t)k * n);
    double *C = calloc((size_t)m * n, sizeof(double));
    if (!A || !Bmat || !C) { perror("alloc"); return 1; }
    for (size_t i = 0; i < (size_t)m * k; ++i) A[i] = rand_double();
    for (size_t i = 0; i < (size_t)k * n; ++i) Bmat[i] = rand_double();

    double t0 = now_seconds();
    for (int ii = 0; ii < m; ii += B) {
        int imax = (ii + B > m) ? m : ii + B;
        for (int kk = 0; kk < k; kk += B) {
            int kmax = (kk + B > k) ? k : kk + B;
            for (int jj = 0; jj < n; jj += B) {
                int jmax = (jj + B > n) ? n : jj + B;
                for (int i = ii; i < imax; ++i) {
                    for (int kk2 = kk; kk2 < kmax; ++kk2) {
                        double a = A[(size_t)i * k + kk2];
                        #pragma GCC ivdep
                        for (int j = jj; j < jmax; ++j) {
                            C[(size_t)i * n + j] += a * Bmat[(size_t)kk2 * n + j];
                        }
                    }
                }
            }
        }
    }
    double t1 = now_seconds();
    double checksum = 0.0; for (size_t i = 0; i < (size_t)m * n; ++i) checksum += C[i];
    printf("%d %d %d %d %f %e\n", m, n, k, B, t1 - t0, checksum);
    free(A); free(Bmat); free(C);
    return 0;
}
