#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "philosopher.h"
#include "util.h"

pthread_mutex_t mutex_thread; /* The mutex thread responsible for
 controlling individual forks between the Philosophers. */

static int number_of_times_to_cycle = 1; /* Command-line argument,
 default 1. */

static void lock_and_print_state() {
	// Lock the mutex thread, print the status line, then unlock.
	pthread_mutex_lock(&mutex_thread);
	print_status_line(philosophers, forks);
	pthread_mutex_unlock(&mutex_thread);
}

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
		 * At first, I'm hungry. I want to EAT first.
		 ************************************************/
		// Attempt to pick up forks.
		pick_up_fork(philosopher, philosopher->assigned_left_fork);
		lock_and_print_state();

		pick_up_fork(philosopher, philosopher->assigned_right_fork);
		lock_and_print_state();

		// Switch to EATING state.
		change_state(philosopher, EATING);
		lock_and_print_state();

		// Eat for a bit (sleep the thread).
		dawdle();
		lock_and_print_state();

		// Switch to CHANGING state.
		change_state(philosopher, CHANGING);

		/*************************************************
		 * Now that I've finished EATING, I want to THINK.
		 *************************************************/
		// Attempt to put down forks.
		put_down_fork(philosopher, philosopher->assigned_left_fork);
		lock_and_print_state();

		put_down_fork(philosopher, philosopher->assigned_right_fork);
		lock_and_print_state();

		// Switch to THINKING state.
		change_state(philosopher, THINKING);
		lock_and_print_state();

		// Think for a bit (sleep the thread).
		dawdle();
		lock_and_print_state();
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

	// Initialize the mutex thread.
	if (pthread_mutex_init(&mutex_thread, NULL ) == -1) {
		fprintf(stderr, "pthread_mutex_init failed: %s", strerror(errno));
		exit(-1);
	}

	// Initialize the Philosophers and the Forks.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		philosophers[i].id = i;
		philosophers[i].assigned_left_fork = ((i + 1) % NUM_PHILOSOPHERS);
		philosophers[i].assigned_right_fork = (i % NUM_PHILOSOPHERS);

//		printf("Philosopher %c's assigned left fork is %d\n", i + 'A',
//				philosophers[i].assigned_left_fork);
//		printf("Philosopher %c's assigned right fork is %d\n\n", i + 'A',
//				philosophers[i].assigned_right_fork);

		philosophers[i].state = CHANGING;

		forks[i] = -1;
	}

	// Spawn each of the Philosophers' child pthreads.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		// pthread_create() launches a new thread running the function
		// child(), passes a pointer to the argument in id[i], and places
		// a thread identifier in childid[i].
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

	// Now wait for each child thread to finish.
	// Note: Unlike the original trivial, pthread_join() requires us to
	// name a specific thread to wait for, thus the children will always
	// join in the same order regardless of when they actually terminate.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {

		// This parent thread will wait for each child thread in the order
		// of this array.
		int res = pthread_join(philosophers[i].thread, NULL );

		// Error check on the pthread_join() call.
		if (res == -1) {
			fprintf(stderr, "Child %i:	%s\n", i, strerror(errno));
			exit(-1);
		}

//		printf("|  Child %d exited.\n", i);
	}

	// Print the footer in the parent thread.
	print_footer();

	// Destroy the mutex thread.
	if (pthread_mutex_destroy(&mutex_thread) == -1) {
		fprintf(stderr, "pthread_mutex_init failed: %s", strerror(errno));
		exit(-1);
	}

	return 0;  // Exit successfully.
}
