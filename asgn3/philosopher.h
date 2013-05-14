#include <pthread.h>

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#define NUM_PHILOSOPHERS 5

typedef struct {
	pthread_mutex_t thread;
	int left_fork;
	int right_fork;
} Philosopher;

#endif
