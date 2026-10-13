#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#define SCHED_STATIC  0
#define SCHED_DYNAMIC 1

void parallel_for(int start, int end, int inc,
                  void *(*functor)(int, void *), void *arg,
                  int num_threads);

void parallel_for_sched(int start, int end, int inc,
                        void *(*functor)(int, void *), void *arg,
                        int num_threads, int schedule, int chunk_size);

#endif
