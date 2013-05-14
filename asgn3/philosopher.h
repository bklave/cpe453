#include <pthread.h>
#include "util.h"

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#define NUM_PHILOSOPHERS 5

typedef enum {
	EATING, THINKING, CHANGING
} State;

typedef struct {
	int id; /* The ID of this Philosopher */

	pthread_t thread; /* The pthread used to actually run Philosopher
	 logic. */

	int left_fork; /* Integer representing which fork the Philosopher has
	 in his left hand, or -1 for none. */

	int right_fork; /* Integer representing which fork the Philosopher has
	 in his right hand, or -1 for none. */

	State state; /* Enum for the State that the Philosopher is currently
	 in (EATING, THINKING, or CHANGING) */

	bool is_hungry; /* If this Philosopher is_hungry, then he will attempt
	 to EAT next. If he is not hungry, he will attempt to THINK next. */
} Philosopher;

void print_status_line(Philosopher philosophers_to_print[]);

//void sanity_check_all_philosophers(Philosopher philosophers_to_sanity_check[]);

void change_state(Philosopher philosopher, State new_state);

void pick_up_fork(Philosopher philosopher, int fork);

void put_down_fork(Philosopher philosopher, int fork);

#endif
