#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "philosopher.h"
#include "util.h"

#define DEBUG 0

static int number_of_times_to_cycle = 1; /* Command-line argument,
 default 1. */

void *eat_think_cycle(void *arg) {
	/*
	 * This function will be executed as the body of each child thread.
	 * It expects a single parameter that is an int (an ID for the
	 * Philosopher).
	 *
	 * The parameter is a void * to comply with the prototype, but we know
	 * what's really in there.
	 */
	int philosopher_id = *(int *) arg;
	Philosopher *philosopher = &philosophers[philosopher_id];
	int i = 0;

	for (i = 0; i < number_of_times_to_cycle; i++) {
		/************************************************
		 * At first, I'm hungry. I want to EAT.
		 ************************************************/
		// Attempt to pick up forks.
		pick_up_fork(philosopher, &forks[philosopher->assigned_left_fork]);
		pick_up_fork(philosopher, &forks[philosopher->assigned_right_fork]);

		// Switch to EATING state.
		change_state(philosopher, EATING);

		// Eat for a bit (sleep the thread).
		dawdle(philosopher);

		// Switch to CHANGING state.
		change_state(philosopher, CHANGING);

		/*************************************************
		 * Now that I've finished EATING, I want to THINK.
		 *************************************************/
		// Attempt to put down forks.
		put_down_fork(philosopher, &forks[philosopher->assigned_left_fork]);
		put_down_fork(philosopher, &forks[philosopher->assigned_right_fork]);

		// Switch to THINKING state.
		change_state(philosopher, THINKING);

		// Think for a bit (sleep the thread).
		dawdle(philosopher);
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
		fprintf(stderr, "pthread_mutex_init failed: %s", strerror(errno));
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
		philosophers[i].id = i;
		philosophers[i].assigned_left_fork = ((i + 1) % NUM_PHILOSOPHERS);
		philosophers[i].assigned_right_fork = (i % NUM_PHILOSOPHERS);

#if DEBUG
		printf("Philosopher %c's assigned left fork is %d\n", i + 'A',
				philosophers[i].assigned_left_fork);
		printf("Philosopher %c's assigned right fork is %d\n\n", i + 'A',
				philosophers[i].assigned_right_fork);
#endif

		philosophers[i].state = CHANGING;
	}

	// Spawn each of the Philosophers' loop logic pthreads.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		// pthread_create() launches a new thread running the function
		// eat_think_cycle(), passes a pointer to the argument in id[i],
		// and places a thread identifier in childid[i].
		int res;
		res = pthread_create(&philosophers[i].thread, NULL, eat_think_cycle,
				(void *) (&ids[i]) // Pass the Philosopher object.
				);

		// Error check.
		if (res == -1) {
			fprintf(stderr, "Child %i:	%s\n", i, strerror(errno));
			exit(-1);
		}
	}

	// Now wait for each Philosopher's thread to finish.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		if (pthread_join(philosophers[i].thread, NULL ) == -1) {
			fprintf(stderr, "Child %i:	%s\n", i, strerror(errno));
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
		fprintf(stderr, "pthread_mutex_init failed: %s", strerror(errno));
		exit(-1);
	}

	return 0;  // Exit successfully.
}
