#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

static int parse_arg_int(int argc, char** argv, const char* key, int default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return atoi(argv[i+1]);
    return default_v;
}

static const char* parse_arg_str(int argc, char** argv, const char* key, const char* default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return argv[i+1];
    return default_v;
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
    int n;
    int partition; 
    int *arr;
    long long *partial;
    barrier_t *barrier_start;
    barrier_t *barrier_partial;
    barrier_t *barrier_finish;
    long long *acc_ptr;
    pthread_mutex_t *acc_mutex;
    int agg; 
    int runs;
} SumArg;

static double timespec_diff_sec(const struct timespec *a, const struct timespec *b) {
    return (b->tv_sec - a->tv_sec) + (b->tv_nsec - a->tv_nsec) * 1e-9;
}

void* sum_worker(void* _arg) {
    SumArg* sa = (SumArg*)_arg;
    int tid = sa->tid;
    int threads = sa->threads;
    int n = sa->n;
    int partition = sa->partition;
    int *arr = sa->arr;
    long long *partial = sa->partial;
    barrier_t *bar_start = sa->barrier_start;
    barrier_t *bar_partial = sa->barrier_partial;
    barrier_t *bar_finish = sa->barrier_finish;
    long long *acc = sa->acc_ptr;
    pthread_mutex_t *acc_mutex = sa->acc_mutex;
    int agg = sa->agg;
    int runs = sa->runs;

    for (int r = 0; r < runs; ++r) {
        barrier_wait(bar_start);
        long long s = 0;
        if (partition == 0) {
            int base = n / threads;
            int rem = n % threads;
            int start = base * tid + (tid < rem ? tid : rem);
            int len = base + (tid < rem ? 1 : 0);
            for (int i = 0; i < len; ++i) s += arr[start + i];
        } else {
            for (int i = tid; i < n; i += threads) s += arr[i];
        }

        if (agg == 0) {
            partial[tid] = s;
        } else if (agg == 1) { 
            pthread_mutex_lock(acc_mutex);
            *acc += s;
            pthread_mutex_unlock(acc_mutex);
        } else { 
            __atomic_fetch_add(acc, s, __ATOMIC_RELAXED);
        }

        barrier_wait(bar_partial);

        if (agg == 0 && tid == 0) {
            long long sum = 0;
            for (int i = 0; i < threads; ++i) sum += partial[i];
            __atomic_store_n(acc, sum, __ATOMIC_RELAXED);
        }

        barrier_wait(bar_finish);
    }
    return NULL;
}

int main(int argc, char** argv) {
    int n = parse_arg_int(argc, argv, "--n", 1000000);
    int threads = parse_arg_int(argc, argv, "--threads", 4);
    int runs = parse_arg_int(argc, argv, "--runs", 3);
    const char* partition_s = parse_arg_str(argc, argv, "--partition", "block");
    int partition = (strcmp(partition_s, "cyclic") == 0) ? 1 : 0;
    const char* agg_s = parse_arg_str(argc, argv, "--agg", "serial");
    int agg = 0;
    if (strcmp(agg_s, "mutex") == 0) agg = 1;
    else if (strcmp(agg_s, "atomic") == 0) agg = 2;
    int seed = parse_arg_int(argc, argv, "--seed", 42);

    if (threads < 1) threads = 1;
    if (n <= 0) { fprintf(stderr, "invalid n\n"); return 1; }

    int *arr = (int*)malloc((size_t)n * sizeof(int));
    if (!arr) { fprintf(stderr, "malloc failed\n"); return 2; }
    srand(seed);
    for (int i = 0; i < n; ++i) arr[i] = rand() & 0xFF; 

    pthread_t *ths = (pthread_t*)malloc(sizeof(pthread_t) * (size_t)threads);
    SumArg *sargs = (SumArg*)malloc(sizeof(SumArg) * (size_t)threads);
    long long *partial = (long long*)malloc(sizeof(long long) * (size_t)threads);
    barrier_t bar_start, bar_partial, bar_finish;
    barrier_init(&bar_start, threads + 1);
    barrier_init(&bar_partial, threads + 1);
    barrier_init(&bar_finish, threads + 1);
    pthread_mutex_t acc_mutex;
    long long acc = 0;
    if (agg == 1) pthread_mutex_init(&acc_mutex, NULL);

    for (int t = 0; t < threads; ++t) {
        sargs[t].tid = t;
        sargs[t].threads = threads;
        sargs[t].n = n;
        sargs[t].partition = partition;
        sargs[t].arr = arr;
        sargs[t].partial = partial;
        sargs[t].barrier_start = &bar_start;
        sargs[t].barrier_partial = &bar_partial;
        sargs[t].barrier_finish = &bar_finish;
        sargs[t].acc_ptr = &acc;
        sargs[t].acc_mutex = &acc_mutex;
        sargs[t].agg = agg;
        sargs[t].runs = runs;
        pthread_create(&ths[t], NULL, sum_worker, &sargs[t]);
    }

    double total_time = 0.0;
    long long result = 0;
    for (int r = 0; r < runs; ++r) {
        __atomic_store_n(&acc, 0, __ATOMIC_RELAXED);
        for (int i = 0; i < threads; ++i) partial[i] = 0;
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        barrier_wait(&bar_start);
        barrier_wait(&bar_partial);
        barrier_wait(&bar_finish);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double elapsed = timespec_diff_sec(&t0, &t1);
        total_time += elapsed;
        result = __atomic_load_n(&acc, __ATOMIC_RELAXED);
    }

    double avg = total_time / runs;
    printf("sum_pthreads partition=%s agg=%s threads=%d n=%d runs=%d time(s)=%.6f sum=%lld\n",
           partition_s, agg == 0 ? "serial" : (agg == 1 ? "mutex" : "atomic"), threads, n, runs, avg, result);

    for (int t = 0; t < threads; ++t) pthread_join(ths[t], NULL);
    barrier_destroy(&bar_start);
    barrier_destroy(&bar_partial);
    barrier_destroy(&bar_finish);
    if (agg == 1) pthread_mutex_destroy(&acc_mutex);

    free(arr); free(ths); free(sargs); free(partial);
    return 0;
}
