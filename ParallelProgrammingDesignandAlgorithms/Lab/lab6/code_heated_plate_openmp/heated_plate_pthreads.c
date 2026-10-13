#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "parallel_for.h"

#define M 500
#define N 500

/* Get wall-clock time in seconds */
static double get_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

/* Shared context for all functors */
struct hp_ctx {
    int rows, cols;
    double *w;
    double *u;
    double *row_diff;   /* per-row max diff, no lock needed */
    double mean;
    double diff;
    pthread_mutex_t mutex;
};

/* ---- 1D boundary functors ---- */

static void *set_left(int i, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    ctx->w[i * ctx->cols + 0] = 100.0;
    return NULL;
}

static void *set_right(int i, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    ctx->w[i * ctx->cols + (ctx->cols - 1)] = 100.0;
    return NULL;
}

static void *set_bottom(int j, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    ctx->w[(ctx->rows - 1) * ctx->cols + j] = 100.0;
    return NULL;
}

static void *set_top(int j, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    ctx->w[0 * ctx->cols + j] = 0.0;
    return NULL;
}

/* ---- Mean reduction functors ---- */

static void *mean_vert(int i, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    double val = ctx->w[i * ctx->cols + 0]
               + ctx->w[i * ctx->cols + (ctx->cols - 1)];
    pthread_mutex_lock(&ctx->mutex);
    ctx->mean += val;
    pthread_mutex_unlock(&ctx->mutex);
    return NULL;
}

static void *mean_horiz(int j, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    double val = ctx->w[(ctx->rows - 1) * ctx->cols + j]
               + ctx->w[0 * ctx->cols + j];
    pthread_mutex_lock(&ctx->mutex);
    ctx->mean += val;
    pthread_mutex_unlock(&ctx->mutex);
    return NULL;
}

/* ---- 2D interior functors (row-parallel, functor handles one row) ---- */

static void *init_row(int i, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    int n = ctx->cols;
    double mean = ctx->mean;
    for (int j = 1; j < n - 1; j++) {
        ctx->w[i * n + j] = mean;
    }
    return NULL;
}

static void *copy_row(int i, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    int n = ctx->cols;
    for (int j = 0; j < n; j++) {
        ctx->u[i * n + j] = ctx->w[i * n + j];
    }
    return NULL;
}

static void *compute_row(int i, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    int n = ctx->cols;
    double *w = ctx->w;
    double *u = ctx->u;
    for (int j = 1; j < n - 1; j++) {
        w[i * n + j] = (u[(i - 1) * n + j] + u[(i + 1) * n + j]
                      + u[i * n + (j - 1)] + u[i * n + (j + 1)]) / 4.0;
    }
    return NULL;
}

static void *diff_row(int i, void *arg) {
    struct hp_ctx *ctx = (struct hp_ctx *)arg;
    int n = ctx->cols;
    double local_max = 0.0;
    for (int j = 1; j < n - 1; j++) {
        double d = fabs(ctx->w[i * n + j] - ctx->u[i * n + j]);
        if (d > local_max) local_max = d;
    }
    ctx->row_diff[i] = local_max;
    return NULL;
}

/* ---- Main ---- */

int main(int argc, char *argv[]) {
    int num_threads = 4;
    int schedule = SCHED_STATIC;
    int chunk_size = 1;

    /* Parse command-line arguments */
    if (argc > 1) num_threads = atoi(argv[1]);
    if (argc > 2) schedule = atoi(argv[2]);
    if (argc > 3) chunk_size = atoi(argv[3]);

    double epsilon = 0.001;
    int iterations;
    int iterations_print;

    /* Allocate 2D arrays as flat 1D arrays */
    double *w = (double *)malloc(M * N * sizeof(double));
    double *u = (double *)malloc(M * N * sizeof(double));
    double *row_diff = (double *)malloc(M * sizeof(double));

    struct hp_ctx ctx;
    ctx.rows = M;
    ctx.cols = N;
    ctx.w = w;
    ctx.u = u;
    ctx.row_diff = row_diff;
    ctx.mean = 0.0;
    ctx.diff = 0.0;
    pthread_mutex_init(&ctx.mutex, NULL);

    printf("\n");
    printf("HEATED_PLATE_PTHREADS\n");
    printf("  C/Pthreads version (via parallel_for library)\n");
    printf("  A program to solve for the steady state temperature distribution\n");
    printf("  over a rectangular plate.\n");
    printf("\n");
    printf("  Spatial grid of %d by %d points.\n", M, N);
    printf("  The iteration will be repeated until the change is <= %e\n", epsilon);
    printf("  Number of threads = %d\n", num_threads);
    printf("  Schedule = %s", schedule == SCHED_STATIC ? "static" : "dynamic");
    if (schedule == SCHED_DYNAMIC)
        printf(", chunk_size = %d", chunk_size);
    printf("\n");

    /* ---- Set boundary values ---- */
    parallel_for_sched(1, M - 1, 1, set_left, &ctx, num_threads, schedule, chunk_size);
    parallel_for_sched(1, M - 1, 1, set_right, &ctx, num_threads, schedule, chunk_size);
    parallel_for_sched(0, N, 1, set_bottom, &ctx, num_threads, schedule, chunk_size);
    parallel_for_sched(0, N, 1, set_top, &ctx, num_threads, schedule, chunk_size);

    /* ---- Compute mean of boundary values ---- */
    ctx.mean = 0.0;
    parallel_for_sched(1, M - 1, 1, mean_vert, &ctx, num_threads, schedule, chunk_size);
    parallel_for_sched(0, N, 1, mean_horiz, &ctx, num_threads, schedule, chunk_size);
    ctx.mean = ctx.mean / (double)(2 * M + 2 * N - 4);
    printf("\n");
    printf("  MEAN = %f\n", ctx.mean);

    /* ---- Initialize interior to mean value ---- */
    parallel_for_sched(1, M - 1, 1, init_row, &ctx, num_threads, schedule, chunk_size);

    /* ---- Iterative solver ---- */
    iterations = 0;
    iterations_print = 1;
    printf("\n");
    printf(" Iteration  Change\n");
    printf("\n");

    double wtime = get_time();
    ctx.diff = epsilon;

    while (epsilon <= ctx.diff) {
        /* Save old solution */
        parallel_for_sched(0, M, 1, copy_row, &ctx, num_threads, schedule, chunk_size);

        /* Compute new estimate */
        parallel_for_sched(1, M - 1, 1, compute_row, &ctx, num_threads, schedule, chunk_size);

        /* Compute max difference (rows write to row_diff[], serial reduce) */
        ctx.diff = 0.0;
        parallel_for_sched(1, M - 1, 1, diff_row, &ctx, num_threads, schedule, chunk_size);
        for (int r = 1; r < M - 1; r++) {
            if (ctx.row_diff[r] > ctx.diff) ctx.diff = ctx.row_diff[r];
        }

        iterations++;
        if (iterations == iterations_print) {
            printf("  %8d  %f\n", iterations, ctx.diff);
            iterations_print = 2 * iterations_print;
        }
    }
    wtime = get_time() - wtime;

    printf("\n");
    printf("  %8d  %f\n", iterations, ctx.diff);
    printf("\n");
    printf("  Error tolerance achieved.\n");
    printf("  Wallclock time = %f\n", wtime);

    printf("\n");
    printf("HEATED_PLATE_PTHREADS:\n");
    printf("  Normal end of execution.\n");

    pthread_mutex_destroy(&ctx.mutex);
    free(w);
    free(u);
    free(row_diff);

    return 0;
}
