#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "philosopher.h"

void print_current_state(Philosopher philosophers_to_print[]) {
	int phil = 0, fork = 0;

	// Sanity check the Philosophers you were given.
//	sanity_check_philosophers(philosophers_to_print);

	for (phil = 0; phil < NUM_PHILOSOPHERS; phil++) {

		printf("| ");

		for (fork = 0; fork < NUM_PHILOSOPHERS; fork++) {
			if (philosophers_to_print[phil].left_fork == fork) {
				printf("%d", fork);
			} else if (philosophers_to_print[phil].right_fork == fork) {
				printf("%d", fork);
			} else {
				printf("-");
			}
		}

		printf("       ");
	}
	printf("|\n");
}

void sanity_check_philosophers(Philosopher philosophers_to_sanity_check[]) {
	// Data structure representing each fork, and who is currently holding
	// the fork.
	int forks[NUM_PHILOSOPHERS];
	int i = 0;
	int left_fork = -1, right_fork = -1;

	// Set each of the forks to set their owner to be -1.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		forks[i] = -1;
	}

	// Perform sanity checks on each of the philosophers.
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		// Reference the forks that each philosopher claims he has
		// ownership of.
		left_fork = philosophers_to_sanity_check[i].left_fork;
		right_fork = philosophers_to_sanity_check[i].right_fork;

		// Assert that those forks truly are unused.
		if (forks[left_fork] == -1) {
			fprintf(stderr, "Philosopher %d claims fork %d as his left fork, "
					"but it is already taken by Philosopher %d", i, left_fork,
					forks[left_fork]);
			exit(-1);
		}
		if (forks[right_fork] == -1) {
			fprintf(stderr, "Philosopher %d claims fork %d as his right fork, "
					"but it is already taken by Philosopher %d", i, left_fork,
					forks[left_fork]);
			exit(-1);
		}

		// If everything is okay, then set the forks to be used by this
		// philosopher.
		forks[left_fork] = i;
		forks[right_fork] = i;
	}
}
