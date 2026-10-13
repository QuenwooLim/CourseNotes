/**
 * @author RookieHPC
 * @brief Original source code at https://rookiehpc.org/openmp/docs/dynamic/index.html
 **/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/**
 * @brief Illustrates the dynamic scheduling distribution.
 * @details A for loop is parallelised across 2 threads using the dynamic
 * policy, in 2 situations:
 *     1) No chunksize passed
 *     2) A chunksize of 2 iterations
 **/
int main(int argc, char* argv[])
{
	// Use 2 threads when creating OpenMP parallel regions
	omp_set_num_threads(2);

	printf("With no chunksize passed:\n");

	// Parallelise the for loop using the dynamic schedule
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < 10; i++)
	{
		printf("Thread %d processes iteration %d.\n", omp_get_thread_num(), i);
	}

	printf("With a chunksize of 2:\n");

	// Parallelise the for loop using the dynamic schedule and chunks of 2 iterations
	#pragma omp parallel for schedule(dynamic, 2)
	for(int i = 0; i < 10; i++)
	{
		printf("Thread %d processes iteration %d.\n", omp_get_thread_num(), i);
	}

	return EXIT_SUCCESS;
}
