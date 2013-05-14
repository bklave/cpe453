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
		philosopher->state = new_state;
		break;
	case THINKING:
		philosopher->state = new_state;
		break;
	case CHANGING:
		philosopher->state = new_state;
		break;
	default:
		perror("Attempted to change to unknown state.");
		exit(-1);
	}

	// Print state.
	print_global_state();
}

void pick_up_forks(Philosopher *philosopher) {
	Fork *left_fork = NULL, *right_fork = NULL;

	left_fork = &forks[philosopher->assigned_left_fork];
	right_fork = &forks[philosopher->assigned_right_fork];

	// Try to lock the left fork.
	if (pthread_mutex_lock(&left_fork->mutex_lock) != 0) {
		perror("mutex_lock");
		exit(-1);
	}

	// Try to lock the right fork.
	if (pthread_mutex_lock(&right_fork->mutex_lock) != 0) {
		perror("mutex_lock");
		exit(-1);
	}

	left_fork->owner = philosopher;
	print_global_state();

	right_fork->owner = philosopher;
	print_global_state();

//	// Assert that this Fork's current owner is NULL.
//	if (fork->owner != NULL ) {
//		fprintf(stderr, "When Philosopher %c attempted to pick up "
//				"Fork %d, Fork %d's owner was Philosopher %c and it "
//				"should have been NULL", philosopher->id + 'A', fork->id,
//				fork->id, fork->owner->id + 'A');
//		exit(-1);
//	}
//
//	// Set the Fork's owner to this Philosopher.
//	fork->owner = philosopher;

}

void put_down_forks(Philosopher *philosopher) {
	Fork *left_fork = NULL, *right_fork = NULL;

	left_fork = &forks[philosopher->assigned_left_fork];
	right_fork = &forks[philosopher->assigned_right_fork];

//	// Assert that this Fork's current owner is indeed this Philosopher.
//	if (lef->owner != philosopher) {
//		fprintf(stderr, "When Philosopher %c attempted to put down "
//				"Fork %d, Fork %d's owner was NOT Philosopher %c. "
//				"It was Philosopher %c instead.", philosopher->id + 'A',
//				fork->id, fork->id, philosopher->id + 'A',
//				fork->owner->id + 'A');
//		exit(-1);
//	}

	// Set the Fork's owner to NULL.
	left_fork->owner = NULL;
	print_global_state();

	right_fork->owner = NULL;
	print_global_state();

	// Unlock the left fork
	if (pthread_mutex_unlock(&left_fork->mutex_lock) != 0) {
		perror("mutex_unlock");
		exit(-1);
	}

	// Unlock the right fork
	if (pthread_mutex_unlock(&right_fork->mutex_lock) != 0) {
		perror("mutex_unlock");
		exit(-1);
	}
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
