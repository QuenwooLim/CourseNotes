#include <pthread.h>
#include <stdlib.h>
#include "parallel_for.h"

#define MAX_THREADS 128

/* ---- Persistent thread pool ---- */

static pthread_t pool[MAX_THREADS];
static int pool_size = 0;
static int pool_active = 0;

static pthread_mutex_t pool_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t work_ready = PTHREAD_COND_INITIALIZER;
static pthread_cond_t work_done  = PTHREAD_COND_INITIALIZER;

static volatile int job_gen = 0;       /* increments each call */
static int worker_gen[MAX_THREADS];    /* last generation each worker processed */
static volatile int num_done = 0;
static volatile int shutdown = 0;

/* Work parameters for the current job */
static int w_start, w_end, w_inc;
static void *(*w_functor)(int, void *);
static void *w_arg;
static int w_schedule;
static int w_chunk_size;
static volatile int w_next_iter;

/* ---- Worker functions ---- */

static void do_static_work(int tid, int nthreads) {
    int total = (w_end - w_start + w_inc - 1) / w_inc;
    if (total <= 0) return;

    int per = total / nthreads;
    int rem = total % nthreads;

    int my_start, my_count;
    if (tid < rem) {
        my_start = w_start + tid * (per + 1) * w_inc;
        my_count = per + 1;
    } else {
        my_start = w_start + (rem * (per + 1) + (tid - rem) * per) * w_inc;
        my_count = per;
    }

    for (int k = 0; k < my_count; k++) {
        int idx = my_start + k * w_inc;
        if (idx >= w_end) break;
        w_functor(idx, w_arg);
    }
}

static void do_dynamic_work(int chunk) {
    while (1) {
        pthread_mutex_lock(&pool_mutex);
        int my_start = w_next_iter;
        if (my_start >= w_end) {
            pthread_mutex_unlock(&pool_mutex);
            break;
        }
        int my_end = my_start + chunk * w_inc;
        if (my_end > w_end) my_end = w_end;
        w_next_iter = my_end;
        pthread_mutex_unlock(&pool_mutex);

        for (int idx = my_start; idx < my_end; idx += w_inc) {
            w_functor(idx, w_arg);
        }
    }
}

static void *pool_worker(void *arg) {
    int tid = *(int *)arg;
    free(arg);

    while (1) {
        pthread_mutex_lock(&pool_mutex);

        /* Wait for a new job (generation > last processed) */
        while (worker_gen[tid] >= job_gen && !shutdown) {
            pthread_cond_wait(&work_ready, &pool_mutex);
        }

        if (shutdown) {
            pthread_mutex_unlock(&pool_mutex);
            return NULL;
        }

        int active = (tid < pool_active);
        pthread_mutex_unlock(&pool_mutex);

        if (active) {
            if (w_schedule == SCHED_DYNAMIC) {
                do_dynamic_work(w_chunk_size);
            } else {
                do_static_work(tid, pool_active);
            }
        }

        /* Mark this job as processed and report completion */
        pthread_mutex_lock(&pool_mutex);
        worker_gen[tid] = job_gen;
        num_done++;
        if (num_done >= pool_size) {
            pthread_cond_signal(&work_done);
        }
        pthread_mutex_unlock(&pool_mutex);
    }
}

/* ---- Pool management ---- */

static void ensure_pool(int num_threads) {
    if (num_threads > MAX_THREADS) num_threads = MAX_THREADS;
    if (pool_size >= num_threads) return;

    pthread_mutex_lock(&pool_mutex);
    for (int i = pool_size; i < num_threads; i++) {
        int *tid = (int *)malloc(sizeof(int));
        *tid = i;
        worker_gen[i] = job_gen;  /* start at current generation */
        pthread_create(&pool[i], NULL, pool_worker, tid);
    }
    pool_size = num_threads;
    pthread_mutex_unlock(&pool_mutex);
}

/* ---- Public API ---- */

void parallel_for_sched(int start, int end, int inc,
                        void *(*functor)(int, void *), void *arg,
                        int num_threads, int schedule, int chunk_size) {
    if (num_threads <= 0) num_threads = 1;
    if (chunk_size <= 0) chunk_size = 1;
    if (inc <= 0) return;
    if (start >= end) return;

    ensure_pool(num_threads);

    pthread_mutex_lock(&pool_mutex);

    /* Post a new job */
    job_gen++;
    w_start    = start;
    w_end      = end;
    w_inc      = inc;
    w_functor  = functor;
    w_arg      = arg;
    w_schedule = schedule;
    w_chunk_size = chunk_size;
    w_next_iter = start;

    pool_active = num_threads;
    num_done    = 0;

    pthread_cond_broadcast(&work_ready);

    while (num_done < pool_size) {
        pthread_cond_wait(&work_done, &pool_mutex);
    }

    pthread_mutex_unlock(&pool_mutex);
}

void parallel_for(int start, int end, int inc,
                  void *(*functor)(int, void *), void *arg,
                  int num_threads) {
    parallel_for_sched(start, end, inc, functor, arg, num_threads, SCHED_STATIC, 1);
}
