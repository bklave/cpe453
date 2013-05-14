#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "philosopher.h"
#include "util.h"

#define NUM_CHILDREN 4


Philosopher philosophers[NUM_PHILOSOPHERS];

void *child(void *id) {
	/*
	 * This function will be executed as the body of each child thread.
	 * It expects a single parameter that is a pointer to an integer, its
	 * ID.
	 *
	 * The parameter is a void * to comply with the prototype, but we know
	 * what's really in there.
	 */
	int who_am_i = *(int *) id;  // Numeric ID.

	printf("Child %d (%d):		Hello.\n\n", who_am_i, (int) getpid());
	printf("Child %d (%d):		Goodbye.\n\n", who_am_i, (int) getpid());

	return NULL ; 	// Exits the thread with no final message.
}

int main(int argc, char *argv[]) {
	pid_t ppid;
	int i;

	int id[NUM_CHILDREN];		// Individual identifiers (see below).
	pthread_t childid[NUM_CHILDREN]; 	// ctivations for each child.

	// Initialize philosophers[]
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		// TODO: Initialize philosopher's thread.
//		philosophers[i].thread = NULL;

		philosophers[i].left_fork = -1;
		philosophers[i].right_fork = -1;
	}


	// Initialize the parent process ID for later use.
	ppid = getpid();

	// Initialize an array of ID numbers for the children. It would be
	// tempting to just pass the loop index (like we did with trivial),
	// but a pointer is passed to the new thread, not the argument itself.
	// Because the loop index will change, the effect is not what one
	// would hope. So, we give each child its own _independent_ ID in the
	// ID array.
	for (i = 0; i < NUM_CHILDREN; i++) {
		id[i] = i;
	}

	// Spawn all the children.
	for (i = 0; i < NUM_CHILDREN; i++) {
		// pthread_create() launches a new thread running the function
		// child(), passes a pointer to the argument in id[i], and places
		// a thread identifier in childid[i].
		//
		// A note on C: below, I write "&childid[i]" to indicate the
		// address of the i-th element of the array child, but I could
		// just as well used pointer arithmetic and written "childid+i".
		int res;
		res = pthread_create(&childid[i], // Where to put the identifier.
				NULL, // Don't set any special properties.
				child, // Call the function child().
				(void *) (&id[i]) // Pass the address of id[i].
				);

		// If there was an error,
		if (res == -1) {
			// report the error condition.
			fprintf(stderr, "Child %i:	%s\n", i, strerror(errno));
			exit(-1);
		}
	}

	// Say hello.
	printf("Parent   (%d):		Hello.\n\n", (int) ppid);

	// Now wait for each child thread to finish.
	//
	// Note: Unlike the original trivial, pthread_join() requires us to
	// name a specific thread to wait for, thus the children will always
	// join in the same order regardless of when they actually terminate.
	for (i = 0; i < NUM_CHILDREN; i++) {
		pthread_join(childid[i], NULL );
		printf("Parent  (%d):		child %d exited.\n\n", (int) ppid, i);
	}

	// Say goodbye.
	printf("Parent   (%d):		Goodbye.\n\n", (int) ppid);

	return 0;  // Exit successfully.
}
