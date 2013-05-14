#include <pthread.h>

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#define NUM_PHILOSOPHERS 5

typedef struct {
	pthread_t temp_thread;
	pthread_mutex_t thread;
	int left_fork;
	int right_fork;
} Philosopher;


void print_current_state(Philosopher philosophers_to_print[]);

void sanity_check_philosophers(Philosopher philosophers_to_sanity_check[]);


#endif
