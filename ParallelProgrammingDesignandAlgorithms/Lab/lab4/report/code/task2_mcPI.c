#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

static int parse_arg_int(int argc, char** argv, const char* key, int default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return atoi(argv[i+1]);
    return default_v;
}

static long long parse_arg_ll(int argc, char** argv, const char* key, long long default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return atoll(argv[i+1]);
    return default_v;
}

static const char* parse_arg_str(int argc, char** argv, const char* key, const char* default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return argv[i+1];
    return default_v;
}

typedef struct {
    int tid;
    int threads;
    long long n;
    long long local_count;
    double local_time;
    unsigned int seed;
} MCArg;

static double timespec_diff_sec(const struct timespec *a, const struct timespec *b) {
    return (b->tv_sec - a->tv_sec) + (b->tv_nsec - a->tv_nsec) * 1e-9;
}

void* mc_worker(void* _arg) {
    MCArg* ma = (MCArg*)_arg;
    int tid = ma->tid;
    int threads = ma->threads;
    long long n = ma->n;
    long long base = n / threads;
    int rem = (int)(n % threads);
    long long len = base + (tid < rem ? 1 : 0);
    unsigned int seed = ma->seed;
    long long cnt = 0;
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (long long i = 0; i < len; ++i) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x*x + y*y <= 1.0) cnt++;
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    ma->local_count = cnt;
    ma->local_time = timespec_diff_sec(&t0, &t1);
    return NULL;
}

int main(int argc, char** argv) {
    long long n = parse_arg_ll(argc, argv, "--n", 65536);
    int threads = parse_arg_int(argc, argv, "--threads", 4);
    int runs = parse_arg_int(argc, argv, "--runs", 5);
    const char* out_trials = parse_arg_str(argc, argv, "--out-trials", "results_pi_trials.csv");

    if (threads < 1 || threads > 16) {
        fprintf(stderr, "Threads must be in [1,16]\n");
        return 2;
    }
    if (n < 1024 || n > 65536) {
        fprintf(stderr, "n must be in [1024,65536]\n");
        return 2;
    }

    pthread_t *ths = (pthread_t*)malloc(sizeof(pthread_t) * (size_t)threads);
    MCArg *args = (MCArg*)malloc(sizeof(MCArg) * (size_t)threads);

    int need_header = 0;
    FILE *fcheck = fopen(out_trials, "r");
    if (!fcheck) need_header = 1; else fclose(fcheck);
    FILE *fout = fopen(out_trials, "a");
    if (!fout) {
        fprintf(stderr, "Failed to open %s for writing\n", out_trials);
        return 2;
    }
    if (need_header) {
        fprintf(fout, "threads,n,trial,time_s,in_circle,pi,per_thread_counts,per_thread_times\n");
        fflush(fout);
    }

    double *trial_times = (double*)malloc(sizeof(double) * (size_t)runs);
    double *trial_pi = (double*)malloc(sizeof(double) * (size_t)runs);

    for (int r = 0; r < runs; ++r) {
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        for (int t = 0; t < threads; ++t) {
            args[t].tid = t;
            args[t].threads = threads;
            args[t].n = n;
            args[t].local_count = 0;
            args[t].local_time = 0.0;
            args[t].seed = (unsigned int)(time(NULL) ^ (t * 2654435761u) ^ (uintptr_t)pthread_self() ^ (r*1234567));
            pthread_create(&ths[t], NULL, mc_worker, &args[t]);
        }
        long long total_in = 0;
        for (int t = 0; t < threads; ++t) {
            pthread_join(ths[t], NULL);
            total_in += args[t].local_count;
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double elapsed = timespec_diff_sec(&t0, &t1);
        double pi = 4.0 * (double)total_in / (double)n;
        trial_times[r] = elapsed;
        trial_pi[r] = pi;

        char counts_buf[4096]; counts_buf[0]='\0';
        char times_buf[4096]; times_buf[0]='\0';
        for (int t = 0; t < threads; ++t) {
            char tmp[128];
            snprintf(tmp, sizeof(tmp), "%lld", (long long)args[t].local_count);
            strncat(counts_buf, tmp, sizeof(counts_buf)-strlen(counts_buf)-1);
            if (t+1 < threads) strncat(counts_buf, ";", sizeof(counts_buf)-strlen(counts_buf)-1);
            snprintf(tmp, sizeof(tmp), "%.6f", args[t].local_time);
            strncat(times_buf, tmp, sizeof(times_buf)-strlen(times_buf)-1);
            if (t+1 < threads) strncat(times_buf, ";", sizeof(times_buf)-strlen(times_buf)-1);
        }

        fprintf(fout, "%d,%lld,%d,%.6f,%lld,%.10f,%s,%s\n",
                threads, n, r+1, elapsed, total_in, pi, counts_buf, times_buf);
        fflush(fout);
    }

    double mean_time = 0.0, mean_pi = 0.0;
    for (int r = 0; r < runs; ++r) { mean_time += trial_times[r]; mean_pi += trial_pi[r]; }
    mean_time /= runs; mean_pi /= runs;
    double var_time = 0.0, var_pi = 0.0;
    for (int r = 0; r < runs; ++r) { var_time += (trial_times[r]-mean_time)*(trial_times[r]-mean_time); var_pi += (trial_pi[r]-mean_pi)*(trial_pi[r]-mean_pi); }
    double std_time = (runs>1)? sqrt(var_time/(runs-1)) : 0.0;
    double std_pi = (runs>1)? sqrt(var_pi/(runs-1)) : 0.0;

    printf("SUMMARY,%d,%lld,%d,%.6f,%.6f,%.10f,%.10f\n",
           threads, n, runs, mean_time, std_time, mean_pi, std_pi);

    fclose(fout);
    free(trial_times); free(trial_pi);
    free(ths); free(args);
    return 0;
}
