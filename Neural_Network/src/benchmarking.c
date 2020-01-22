#include "benchmarking.h"


#define ONE_BILLION 1000000000


// Simple function, but may give erroneous results if multithreading is used:
double time_elapsed(clock_t start)
{
	return (double) (clock() - start) / CLOCKS_PER_SEC;
}


// Write the current time in the given struct:
void get_time(struct timespec *current_time)
{
	clock_gettime(CLOCK_MONOTONIC, current_time);
}


// More precise timer: (POSIX)
double precise_time(struct timespec *start, struct timespec *finish)
{
	long seconds = finish -> tv_sec - start -> tv_sec;
	long ns = finish -> tv_nsec - start -> tv_nsec;

	if (start -> tv_nsec > finish -> tv_nsec) // clock underflow
	{
		--seconds;
		ns += ONE_BILLION;
	}

	return seconds + (double) ns / ONE_BILLION;
}
