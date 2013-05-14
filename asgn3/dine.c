#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "philosopher.h"
#include "util.h"

void *child(void *id) {
	Philosopher *philosopher = (Philosopher *) id;

	/*
	 * This function will be executed as the body of each child thread.
	 * It expects a single parameter that is a pointer to an integer, its
	 * ID.
	 *
	 * The parameter is a void * to comply with the prototype, but we know
	 * what's really in there.
	 */
	int who_am_i = philosopher->id;

	printf("Child %d (%d):		Hello.\n\n", who_am_i, (int) getpid());
	printf("Child %d (%d):		Goodbye.\n\n", who_am_i, (int) getpid());

	return NULL ; 	// Exits the thread with no final message.
}

int main(int argc, char *argv[]) {
	int i;

	Philosopher philosophers[NUM_PHILOSOPHERS]; // Create Philosophers.

	pthread_mutex_t mutex_thread;

	int number_of_times_to_cycle = 1; // The number of times to cycle.

	if (argc == 2) {
		number_of_times_to_cycle = strtol(argv[1], NULL, 10);
	}

	// Parent thread prints header.
	print_header();

	// Initialize the mutex thread.
	if (pthread_mutex_init(&mutex_thread, NULL ) == -1) {
		fprintf(stderr, "pthread_mutex_init failed: %s", strerror(errno));
		exit(-1);
	}

	// Spawn all the children.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		philosophers[i].id = i;
		philosophers[i].left_fork = -1;
		philosophers[i].right_fork = -1;
		philosophers[i].state = CHANGING;
		philosophers[i].is_hungry = true;

		// pthread_create() launches a new thread running the function
		// child(), passes a pointer to the argument in id[i], and places
		// a thread identifier in childid[i].
		int res;
		res = pthread_create(&philosophers[i].thread, // Where to put the identifier.
				NULL, // Don't set any special properties.
				child, // Call the function child().
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

		int res;
		res = pthread_join(philosophers[i].thread, NULL );

		// Error check.
		if (res == -1) {
			fprintf(stderr, "Child %i:	%s\n", i, strerror(errno));
			exit(-1);
		}

		printf("|  Parent:		child %d exited.\n", i);
		print_status_line(philosophers);
	}

	// Parent thread prints footer.
	print_footer();

	// Destroy the mutex thread.
	if (pthread_mutex_destroy(&mutex_thread) == -1) {
		fprintf(stderr, "pthread_mutex_init failed: %s", strerror(errno));
		exit(-1);
	}

	return 0;  // Exit successfully.
}
