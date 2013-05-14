#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#include "philosopher.h"

#define DEBUG 0

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
			philosopher = &phils[p];
			fork = &forks[f];

			if (fork->owner == philosopher) {
				printf("%d", fork->id);
			} else {
				printf("-");
			}
		}

		// Print the State that this Philosopher is currently in.
		switch (phils[p].state) {
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
					"state: %u", p, phils[p].state);
		}
	}

	// Formatting.
	printf("|\n");

	// Unlock the global mutex thread.
	pthread_mutex_unlock(&global_mutex_lock);
}

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

void pick_up_fork(Philosopher *philosopher, Fork *fork) {

	// Try to lock the left fork.
	if (pthread_mutex_lock(&fork->mutex_lock) != 0) {
		perror("mutex_lock");
		exit(-1);
	}

#if DEBUG
	printf("Philosopher %c locked Fork %d.\n", philosopher->id + 'A',
			fork->id);
#endif

	fork->owner = philosopher;
	print_global_state();
}

void put_down_fork(Philosopher *philosopher, Fork *fork) {

	// Set the Fork's owner to NULL.
	fork->owner = NULL;
	print_global_state();

	// Unlock the left fork
	if (pthread_mutex_unlock(&fork->mutex_lock) != 0) {
		perror("mutex_unlock");
		exit(-1);
	}

#if DEBUG
	printf("Philosopher %c unlocked Fork %d\n", philosopher->id + 'A',
			fork->id);
#endif

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
