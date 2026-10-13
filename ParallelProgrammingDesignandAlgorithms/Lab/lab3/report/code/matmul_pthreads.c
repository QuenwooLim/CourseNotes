#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stddef.h>

static int parse_arg_int(int argc, char** argv, const char* key, int default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return atoi(argv[i+1]);
    return default_v;
}

static const char* parse_arg_str(int argc, char** argv, const char* key, const char* default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return argv[i+1];
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

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int trip_count;
} barrier_t;

static void barrier_init(barrier_t *b, int trip_count) {
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
    b->count = 0;
    b->trip_count = trip_count;
}

static void barrier_wait(barrier_t *b) {
    pthread_mutex_lock(&b->mutex);
    b->count += 1;
    if (b->count >= b->trip_count) {
        b->count = 0;
        pthread_cond_broadcast(&b->cond);
    } else {
        while (pthread_cond_wait(&b->cond, &b->mutex) != 0) { }
    }
    pthread_mutex_unlock(&b->mutex);
}

static void barrier_destroy(barrier_t *b) {
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cond);
}

typedef struct {
    int tid;
    int threads;
    int m, n, k;
    int partition; 
    double *A, *B, *C;
    barrier_t *barrier;
    double *thread_times;
    int runs;
} ThreadArg;

static double timespec_diff_sec(const struct timespec *a, const struct timespec *b) {
    return (b->tv_sec - a->tv_sec) + (b->tv_nsec - a->tv_nsec) * 1e-9;
}

void* worker_func(void* _arg) {
    ThreadArg* ta = (ThreadArg*)_arg;
    int tid = ta->tid;
    int threads = ta->threads;
    int m = ta->m;
    int n = ta->n;
    int k = ta->k;
    double *A = ta->A;
    double *B = ta->B;
    double *C = ta->C;
    int partition = ta->partition;
    barrier_t *bar = ta->barrier;
    int runs = ta->runs;

    for (int r = 0; r < runs; ++r) {
        barrier_wait(bar); 
        struct timespec tt0, tt1;
        clock_gettime(CLOCK_MONOTONIC, &tt0);
        if (partition == 0) {
            int base = m / threads;
            int rem = m % threads;
            int start = base * tid + (tid < rem ? tid : rem);
            int rows = base + (tid < rem ? 1 : 0);
            for (int i = 0; i < rows; ++i) {
                int row = start + i;
                double *rowA = A + (size_t)row * n;
                double *rowC = C + (size_t)row * k;
                for (int j = 0; j < k; ++j) {
                    double s = 0.0;
                    for (int t = 0; t < n; ++t) s += rowA[t] * B[(size_t)t * k + j];
                    rowC[j] = s;
                }
            }
        } else if (partition == 1) {
            for (int row = tid; row < m; row += threads) {
                double *rowA = A + (size_t)row * n;
                double *rowC = C + (size_t)row * k;
                for (int j = 0; j < k; ++j) {
                    double s = 0.0;
                    for (int t = 0; t < n; ++t) s += rowA[t] * B[(size_t)t * k + j];
                    rowC[j] = s;
                }
            }
        } else { 
            int basec = k / threads;
            int remc = k % threads;
            int startc = basec * tid + (tid < remc ? tid : remc);
            int cols = basec + (tid < remc ? 1 : 0);
            for (int row = 0; row < m; ++row) {
                double *rowA = A + (size_t)row * n;
                for (int jc = 0; jc < cols; ++jc) {
                    int col = startc + jc;
                    double s = 0.0;
                    for (int t = 0; t < n; ++t) s += rowA[t] * B[(size_t)t * k + col];
                    C[(size_t)row * k + col] = s;
                }
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &tt1);
        if (ta->thread_times) ta->thread_times[tid] += (tt1.tv_sec - tt0.tv_sec) + (tt1.tv_nsec - tt0.tv_nsec) * 1e-9;
        barrier_wait(bar); 
    }
    return NULL;
}

int main(int argc, char** argv) {
    int m = parse_arg_int(argc, argv, "--m", 512);
    int n = parse_arg_int(argc, argv, "--n", 512);
    int k = parse_arg_int(argc, argv, "--k", 512);
    int threads = parse_arg_int(argc, argv, "--threads", 4);
    int runs = parse_arg_int(argc, argv, "--runs", 3);
    const char* partition_s = parse_arg_str(argc, argv, "--partition", "block");
    int partition = 0;
    if (strcmp(partition_s, "cyclic") == 0) partition = 1;
    else if (strcmp(partition_s, "block_col") == 0) partition = 2;
    int save = parse_arg_int(argc, argv, "--save", 0);
    int seed = parse_arg_int(argc, argv, "--seed", 42);

    if (threads < 1) threads = 1;

    if (m <= 0 || n <= 0 || k <= 0) {
        fprintf(stderr, "invalid matrix size\n");
        return 1;
    }

    size_t Asz = (size_t)m * n;
    size_t Bsz = (size_t)n * k;
    size_t Csz = (size_t)m * k;

    double *A = (double*)malloc(Asz * sizeof(double));
    double *B = (double*)malloc(Bsz * sizeof(double));
    double *C = (double*)malloc(Csz * sizeof(double));
    if (!A || !B || !C) { fprintf(stderr, "malloc failed\n"); return 2; }
    srand(seed);
    for (size_t i = 0; i < Asz; ++i) A[i] = ((double)rand()) / RAND_MAX;
    for (size_t i = 0; i < Bsz; ++i) B[i] = ((double)rand()) / RAND_MAX;

    pthread_t *ths = (pthread_t*)malloc(sizeof(pthread_t) * (size_t)threads);
    ThreadArg *targs = (ThreadArg*)malloc(sizeof(ThreadArg) * (size_t)threads);
    double *thread_times = (double*)calloc((size_t)threads, sizeof(double));
    barrier_t barrier;
    barrier_init(&barrier, threads + 1);

    for (int t = 0; t < threads; ++t) {
        targs[t].tid = t;
        targs[t].threads = threads;
        targs[t].m = m; targs[t].n = n; targs[t].k = k;
        targs[t].partition = partition;
        targs[t].A = A; targs[t].B = B; targs[t].C = C;
        targs[t].barrier = &barrier;
        targs[t].thread_times = thread_times;
        targs[t].runs = runs;
        pthread_create(&ths[t], NULL, worker_func, &targs[t]);
    }

    double total_time = 0.0;
    for (int r = 0; r < runs; ++r) {
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        barrier_wait(&barrier); 
        barrier_wait(&barrier); 
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double elapsed = timespec_diff_sec(&t0, &t1);
        total_time += elapsed;
    }

    double avg = total_time / runs;
    double thread_max = 0.0;
    for (int t = 0; t < threads; ++t) {
        double v = thread_times[t] / runs;
        if (v > thread_max) thread_max = v;
    }
    printf("matmul_pthreads partition=%s threads=%d m=%d n=%d k=%d runs=%d time(s)=%.6f threadMax(s)=%.6f\n",
           partition_s, threads, m, n, k, runs, avg, thread_max);

    for (int t = 0; t < threads; ++t) pthread_join(ths[t], NULL);
    barrier_destroy(&barrier);

    if (save) {
        write_matrix_bin("A.bin", A, m, n);
        write_matrix_bin("B.bin", B, n, k);
        write_matrix_bin("C.bin", C, m, k);
        printf("Matrices saved to A.bin/B.bin/C.bin\n");
    }

    free(A); free(B); free(C); free(ths); free(targs); free(thread_times);
    return 0;
}
