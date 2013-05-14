#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "philosopher.h"
#include "util.h"

Philosopher philosophers[NUM_PHILOSOPHERS]; /* The Philosophers. */

pthread_mutex_t mutex_thread; /* The mutex thread responsible for
 controlling individual forks between the Philosophers. */

void *eat_think_cycle(void *arg) {
	/*
	 * This function will be executed as the body of each child thread.
	 * It expects a single parameter that is a pointer to the Philosopher.
	 *
	 * The parameter is a void * to comply with the prototype, but we know
	 * what's really in there.
	 */
	Philosopher *philosopher = (Philosopher *) arg;

	// Lock the mutex thread, print the status line, then unlock.
	pthread_mutex_lock(&mutex_thread);
	print_status_line(philosophers);
	pthread_mutex_unlock(&mutex_thread);

	// Exit the pthread.
	pthread_exit(NULL );
}

int main(int argc, char *argv[]) {
	int i = 0;  // Loop counter.
	int number_of_times_to_cycle = 1; // Command-line argument, default 1.

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

	// Initialize all of the Philosophers' static datum.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		philosophers[i].id = i;
		philosophers[i].left_fork = -1;
		philosophers[i].right_fork = -1;
		philosophers[i].state = CHANGING;
		philosophers[i].is_hungry = true;
	}

	// Spawn each of the Philosophers' child pthreads.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		// pthread_create() launches a new thread running the function
		// child(), passes a pointer to the argument in id[i], and places
		// a thread identifier in childid[i].
		int res;
		res = pthread_create(&philosophers[i].thread, NULL, eat_think_cycle,
				(void *) (&philosophers[i]) // Pass the Philosopher object.
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

		printf("|  Child %d exited.\n", i);
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
