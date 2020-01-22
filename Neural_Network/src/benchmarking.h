#ifndef BENCHMARKING_H
#define BENCHMARKING_H


#include <time.h>


// Simple function, but may give erroneous results if multithreading is used:
double time_elapsed(clock_t start);


// Write the current time in the given struct:
void get_time(struct timespec *current_time);


// More precise timer: (POSIX)
double precise_time(struct timespec *start, struct timespec *finish);


#endif
