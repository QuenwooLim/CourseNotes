#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct {
    int tid;
    int num_threads;
    int M, N, K;
    double *A, *B, *C;
} ThreadData;

void *matmul_worker(void *arg) {
    ThreadData *d = (ThreadData *)arg;
    int rows_per_thread = d->M / d->num_threads;
    int remainder = d->M % d->num_threads;
    int start_row, end_row;

    if (d->tid < remainder) {
        start_row = d->tid * (rows_per_thread + 1);
        end_row = start_row + rows_per_thread + 1;
    } else {
        start_row = d->tid * rows_per_thread + remainder;
        end_row = start_row + rows_per_thread;
    }

    for (int i = start_row; i < end_row; i++) {
        for (int k = 0; k < d->N; k++) {
            double aik = d->A[i * d->N + k];
            for (int j = 0; j < d->K; j++) {
                d->C[i * d->K + j] += aik * d->B[k * d->K + j];
            }
        }
    }
    return NULL;
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s M N K num_threads [-q]\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int K = atoi(argv[3]);
    int num_threads = atoi(argv[4]);
    int quiet = (argc >= 6 && strcmp(argv[5], "-q") == 0);

    double *A = (double *)malloc(M * N * sizeof(double));
    double *B = (double *)malloc(N * K * sizeof(double));
    double *C = (double *)malloc(M * K * sizeof(double));

    srand(42);
    for (int i = 0; i < M * N; i++) A[i] = (double)rand() / RAND_MAX;
    for (int i = 0; i < N * K; i++) B[i] = (double)rand() / RAND_MAX;
    for (int i = 0; i < M * K; i++) C[i] = 0.0;

    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    ThreadData *tdata = (ThreadData *)malloc(num_threads * sizeof(ThreadData));

    double start = get_time();

    for (int t = 0; t < num_threads; t++) {
        tdata[t].tid = t;
        tdata[t].num_threads = num_threads;
        tdata[t].M = M;
        tdata[t].N = N;
        tdata[t].K = K;
        tdata[t].A = A;
        tdata[t].B = B;
        tdata[t].C = C;
        pthread_create(&threads[t], NULL, matmul_worker, &tdata[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    double elapsed = get_time() - start;

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

    free(threads);
    free(tdata);
    free(A);
    free(B);
    free(C);
    return 0;
}
