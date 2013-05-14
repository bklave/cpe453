#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "philosopher.h"
#include "util.h"

static int number_of_times_to_cycle = 1; /* Command-line argument,
 default 1. */

#define DEBUG 0

void *cycle(void *arg) {
	/*
	 * This function will be executed as the body of each child thread.
	 * It expects a single parameter that is an int (an ID for the
	 * Philosopher).
	 *
	 * The parameter is a void * to comply with the prototype, but we know
	 * what's really in there.
	 */
	int philosopher_id = *(int *) arg;
	Philosopher *phil = &phils[philosopher_id];
	int i = 0;

	for (i = 0; i < number_of_times_to_cycle; i++) {
		/************************************************
		 * At first, I'm hungry. I want to EAT.
		 ************************************************/
		// Attempt to pick up forks.
		if (phil->id % 2 == 0) {
			// Evens pick up right-hand forks first.
			pick_up_fork(phil, &forks[phil->des_right_fork]);
			pick_up_fork(phil, &forks[phil->des_left_fork]);
		} else {
			// Odds pick up left-hand forks first.
			pick_up_fork(phil, &forks[phil->des_left_fork]);
			pick_up_fork(phil, &forks[phil->des_right_fork]);
		}

		// Switch to EATING state.
		change_state(phil, EATING);

		// Eat for a bit (sleep the thread).
		dawdle(phil);

		// Switch to CHANGING state.
		change_state(phil, CHANGING);

		/*************************************************
		 * Now that I've finished EATING, I want to THINK.
		 *************************************************/
		// Attempt to put down forks.
		put_down_fork(phil, &forks[phil->des_left_fork]);
		put_down_fork(phil, &forks[phil->des_right_fork]);

		// Switch to THINKING state.
		change_state(phil, THINKING);

		// Think for a bit (sleep the thread).
		dawdle(phil);

		// Switch to CHANGING state.
		change_state(phil, CHANGING);
	}

	// Kill the pthread.
	pthread_exit(NULL );
}

int main(int argc, char *argv[]) {
	int i = 0;  // Loop counter.
	int ids[NUM_PHILOSOPHERS];

	// Initialize the IDs array.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		ids[i] = i;
	}

	// Retrieve the command-line argument of how many times to cycle.
	if (argc >= 2) {
		number_of_times_to_cycle = strtol(argv[1], NULL, 10);
	}

	// Print the header in the parent thread.
	print_header();

	// Initialize the global mutex lock.
	if (pthread_mutex_init(&global_mutex_lock, NULL ) == -1) {
		fprintf(stderr, "pthread_mutex_init"
				" failed: %s", strerror(errno));
		exit(-1);
	}

	// Initialize the Forks.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		forks[i].id = i;
		forks[i].owner = NULL;

		// Initialize each mutex lock.
		if (pthread_mutex_init(&forks[i].mutex_lock, NULL ) == -1) {
			fprintf(stderr, "pthread_mutex_init"
					" failed: %s", strerror(errno));
			exit(-1);
		}
	}

	// Initialize the Philosophers.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		phils[i].id = i;
		phils[i].des_left_fork = ((i + 1) % NUM_PHILOSOPHERS);
		phils[i].des_right_fork = (i % NUM_PHILOSOPHERS);

#if DEBUG
		printf("Philosopher %c's assigned "
				"left fork is %d\n", i + 'A',
				phils[i].des_left_fork);
		printf("Philosopher %c's assigned "
				"right fork is %d\n\n", i + 'A',
				phils[i].des_right_fork);
#endif

		phils[i].state = CHANGING;
	}

	// Spawn each of the Philosophers' pthreads.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		int res;
		pthread_t *t = &phils[i].thread;

		res = pthread_create(t, NULL, cycle, (void *) (&ids[i]));

		if (res == -1) {
			fprintf(stderr, "Child %i:	"
					"%s\n", i, strerror(errno));
			exit(-1);
		}
	}

	// Now wait for each Philosopher's thread to finish.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		int res;

		res = pthread_join(phils[i].thread, NULL );
		if (res == -1) {
			perror("join");
			exit(-1);
		}
	}

	// Print the footer in the parent thread.
	print_footer();

	// Destroy the Forks' mutex locks.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		if (pthread_mutex_destroy(&forks[i].mutex_lock) == -1) {
			fprintf(stderr, "pthread_mutex_init "
					"failed: %s", strerror(errno));
			exit(-1);
		}
	}

	// Destroy the global mutex lock.
	if (pthread_mutex_init(&global_mutex_lock, NULL ) == -1) {
		fprintf(stderr, "pthread_mutex_init"
				" failed: %s", strerror(errno));
		exit(-1);
	}

	return 0;  // Exit successfully.
}
