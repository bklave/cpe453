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

	pthread_t logic_thread; /* The pthread used to
	 run overarching Philosopher "Eat-Think" cycle logic. */

	pthread_mutex_t mutex_lock; /* The mutex lock responsible for
	 locking this Philosopher. */

	int assigned_left_fork; /* Integer representing which fork the
	 Philosopher will be holding in his left hand. Don't change this. */
	int assigned_right_fork; /* Integer representing which fork the
	 Philosopher will be holding in his right hand.  Don't change this. */

	State state; /* Enum for the State that the Philosopher is currently
	 in (EATING, THINKING, or CHANGING) */

//	bool is_hungry; /* If this Philosopher is_hungry, then he will attempt
//	 to EAT next. If he is not hungry, he will attempt to THINK next. */
} Philosopher;

Philosopher philosophers[NUM_PHILOSOPHERS]; /* The Philosophers. */
int forks[NUM_PHILOSOPHERS]; /* The Forks. Each fork contains the ID of
 the Philosopher holding it, or -1 for none.*/

pthread_mutex_t global_mutex_lock; /* A global mutex thread responsible
 for ensuring Philosopher state output is clean. */

void print_global_state();
void change_state(Philosopher *philosopher, State new_state);
void pick_up_fork(Philosopher *philosopher, int fork);
void put_down_fork(Philosopher *philosopher, int fork);
void dawdle(Philosopher *philosopher);

#endif
