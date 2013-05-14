#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#include "philosopher.h"

void print_global_state() {
	Philosopher *philosopher = NULL;
	Fork *fork = NULL;
	int p = 0, f = 0;  // Counters.

	// Lock the global mutex thread.
	pthread_mutex_lock(&global_mutex_lock);

	for (p = 0; p < NUM_PHILOSOPHERS; p++) {
		// Formatting.
		printf("| ");

		// Print the forks that this Philosopher has in hand.
		for (f = 0; f < NUM_PHILOSOPHERS; f++) {
			philosopher = &philosophers[p];
			fork = &forks[f];

			if (fork->owner == philosopher) {
				printf("%d", fork->id);
			} else {
				printf("-");
			}
		}

		// Print the State that this Philosopher is currently in.
		switch (philosophers[p].state) {
		case EATING:
			printf(" Eat   ");
			break;
		case THINKING:
			printf(" Think ");
			break;
		case CHANGING:
			printf("       ");
			break;
		default:
			fprintf(stderr, "Error: Philosopher %d has unknown "
					"state: %u", p, philosophers[p].state);
		}
	}

	// Formatting.
	printf("|\n");

	// Unlock the global mutex thread.
	pthread_mutex_unlock(&global_mutex_lock);
}

/**
 * Nico Email:
 *
 * Each philosopher should be a pthread (rather than having a pthread_t),
 * and you should be modelling the forks as binary semaphores.  That is,
 * to take a fork, you pthread_mutex_lock() it, and to release it you
 * pthread_mutex_unlock() it.  If you try to lock a lock that's already
 * locked, the thread attempting that will block until it's unlocked.
 */

void change_state(Philosopher *philosopher, State new_state) {

	switch (new_state) {
	case EATING:

		break;
	case THINKING:

		break;
	case CHANGING:

		break;
	default:
		perror("Attempted to change to unknown state.");
		exit(-1);
	}

	// Print state.
	print_global_state();
}

void pick_up_fork(Philosopher *philosopher, Fork *fork) {

	// Attempt to lock the Fork that you want to pick up. If the Fork is
	// already locked by another Philosopher's thread, then this process
	// will block until that Fork is unlocked.

	// Assert that this Fork's current owner is NULL.

	// Set the Fork's owner to this Philosopher.

	// Print state.
	print_global_state();
}

void put_down_fork(Philosopher *philosopher, Fork *fork) {

	// Assert that this Fork's current owner is indeed this Philosopher.

	// Set the Fork's owner to NULL.

	// Unlock the Fork that you just put down.

	// Print state.
	print_global_state();
}

void dawdle(Philosopher *philosopher) {
	/* Sleep for a random amount of time between 0 and 999 milliseconds.
	 * This routine is somewhat unreliable, since it doesn't take into
	 * account the possibility that the nanosleep could be interrupted for
	 * some legitimate reason.
	 */
	struct timespec tv;

	int msec = (int) (((double) random() / INT_MAX) * 1000);

	tv.tv_sec = 0;
	tv.tv_nsec = 1000000 * msec;

	if (nanosleep(&tv, NULL ) == -1) {
		perror("nanosleep");
	}

	// Print state.
	print_global_state();
}
