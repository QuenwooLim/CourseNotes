#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

static int parse_arg_int(int argc, char** argv, const char* key, int default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return atoi(argv[i+1]);
    return default_v;
}

static double parse_arg_double(int argc, char** argv, const char* key, double default_v) {
    for (int i = 1; i < argc-1; ++i) if (strcmp(argv[i], key) == 0) return atof(argv[i+1]);
    return default_v;
}

typedef struct {
    double a, b, c;
    double delta;
    int delta_ready;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    double r1_real, r1_imag, r2_real, r2_imag;
} Shared;

static void wait_for_delta(Shared* s, double* delta) {
    pthread_mutex_lock(&s->mutex);
    while (!s->delta_ready) {
        pthread_cond_wait(&s->cond, &s->mutex);
    }
    *delta = s->delta;
    pthread_mutex_unlock(&s->mutex);
}

static void compute_root1(const Shared* s, double delta, double* r_real, double* r_imag) {
    if (fabs(s->a) < 1e-12) {
        if (fabs(s->b) < 1e-12) {
            *r_real = NAN;
            *r_imag = NAN;
        } else {
            *r_real = -s->c / s->b;
            *r_imag = 0.0;
        }
        return;
    }
    if (delta >= 0.0) {
        double sd = sqrt(delta);
        *r_real = (-s->b + sd) / (2.0 * s->a);
        *r_imag = 0.0;
    } else {
        double sd = sqrt(-delta);
        *r_real = -s->b / (2.0 * s->a);
        *r_imag = sd / (2.0 * s->a);
    }
}

static void compute_root2(const Shared* s, double delta, double* r_real, double* r_imag) {
    if (fabs(s->a) < 1e-12) {
        if (fabs(s->b) < 1e-12) {
            *r_real = NAN;
            *r_imag = NAN;
        } else {
            *r_real = -s->c / s->b;
            *r_imag = 0.0;
        }
        return;
    }
    if (delta >= 0.0) {
        double sd = sqrt(delta);
        *r_real = (-s->b - sd) / (2.0 * s->a);
        *r_imag = 0.0;
    } else {
        double sd = sqrt(-delta);
        *r_real = -s->b / (2.0 * s->a);
        *r_imag = -sd / (2.0 * s->a);
    }
}

void* thread_delta(void* arg) {
    Shared* s = (Shared*)arg;
    double delta = s->b * s->b - 4.0 * s->a * s->c;
    pthread_mutex_lock(&s->mutex);
    s->delta = delta;
    s->delta_ready = 1;
    pthread_cond_broadcast(&s->cond);
    pthread_mutex_unlock(&s->mutex);
    return NULL;
}

void* thread_root1(void* arg) {
    Shared* s = (Shared*)arg;
    double delta;
    double r_real, r_imag;
    wait_for_delta(s, &delta);
    compute_root1(s, delta, &r_real, &r_imag);
    pthread_mutex_lock(&s->mutex);
    s->r1_real = r_real;
    s->r1_imag = r_imag;
    pthread_mutex_unlock(&s->mutex);
    return NULL;
}

void* thread_root2(void* arg) {
    Shared* s = (Shared*)arg;
    double delta;
    double r_real, r_imag;
    wait_for_delta(s, &delta);
    compute_root2(s, delta, &r_real, &r_imag);
    pthread_mutex_lock(&s->mutex);
    s->r2_real = r_real;
    s->r2_imag = r_imag;
    pthread_mutex_unlock(&s->mutex);
    return NULL;
}

static double timespec_diff_sec(const struct timespec *a, const struct timespec *b) {
    return (b->tv_sec - a->tv_sec) + (b->tv_nsec - a->tv_nsec) * 1e-9;
}

int main(int argc, char** argv) {
    double a = parse_arg_double(argc, argv, "--a", NAN);
    double b = parse_arg_double(argc, argv, "--b", NAN);
    double c = parse_arg_double(argc, argv, "--c", NAN);
    int threads_req = parse_arg_int(argc, argv, "--threads", 3);
    int runs = parse_arg_int(argc, argv, "--runs", 3);

    if (isnan(a) || isnan(b) || isnan(c)) {
        fprintf(stderr, "Usage: %s --a <a> --b <b> --c <c> [--threads N] [--runs R]\n", argv[0]);
        return 1;
    }

    if (a < -100.0 || a > 100.0 || b < -100.0 || b > 100.0 || c < -100.0 || c > 100.0) {
        fprintf(stderr, "Coefficients must be in [-100,100]\n");
        return 2;
    }

    int threads = threads_req;
    if (threads < 1) threads = 1;
    if (threads > 3) threads = 3;

    Shared s;
    s.a = a; s.b = b; s.c = c;
    s.delta_ready = 0;
    pthread_mutex_init(&s.mutex, NULL);
    pthread_cond_init(&s.cond, NULL);

    double total_time = 0.0;
    for (int r = 0; r < runs; ++r) {
        s.delta_ready = 0;
        pthread_t t_delta, t_r1, t_r2;
        struct timespec t0, t1t;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        pthread_create(&t_delta, NULL, thread_delta, &s);
        if (threads >= 2) {
            pthread_create(&t_r1, NULL, thread_root1, &s);
        }
        if (threads >= 3) {
            pthread_create(&t_r2, NULL, thread_root2, &s);
        }

        if (threads < 2) {
            double delta;
            double r_real, r_imag;
            wait_for_delta(&s, &delta);
            compute_root1(&s, delta, &r_real, &r_imag);
            s.r1_real = r_real;
            s.r1_imag = r_imag;
        }
        if (threads < 3) {
            double delta;
            double r_real, r_imag;
            wait_for_delta(&s, &delta);
            compute_root2(&s, delta, &r_real, &r_imag);
            s.r2_real = r_real;
            s.r2_imag = r_imag;
        }

        pthread_join(t_delta, NULL);
        if (threads >= 2) pthread_join(t_r1, NULL);
        if (threads >= 3) pthread_join(t_r2, NULL);
        clock_gettime(CLOCK_MONOTONIC, &t1t);
        double elapsed = timespec_diff_sec(&t0, &t1t);
        total_time += elapsed;
    }

    double avg = total_time / runs;
        printf("quadratic_pthreads threads=%d a=%.6f b=%.6f c=%.6f runs=%d time(s)=%.6f root1_real=%.6f root1_imag=%.6f root2_real=%.6f root2_imag=%.6f\n",
            threads, a, b, c, runs, avg, s.r1_real, s.r1_imag, s.r2_real, s.r2_imag);

    pthread_mutex_destroy(&s.mutex);
    pthread_cond_destroy(&s.cond);
    return 0;
}
