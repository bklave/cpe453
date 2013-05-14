#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "philosopher.h"

void print_status_line(Philosopher philosophers_to_print[], int forks[]) {
	int phil = 0, fork = 0;  // Counters.

	// Sanity check the Philosophers you were given.
//	sanity_check_philosophers(philosophers_to_print);

	for (phil = 0; phil < NUM_PHILOSOPHERS; phil++) {

		// Formatting.
		printf("| ");

//		// Print the forks that this Philosopher has in hand.
//		for (fork = 0; fork < NUM_PHILOSOPHERS; fork++) {
//			if (philosophers_to_print[phil].left_fork == fork) {
//				printf("%d", fork);
//			} else if (philosophers_to_print[phil].right_fork == fork) {
//				printf("%d", fork);
//			} else {
//				printf("-");
//			}
//		}

		for (fork = 0; fork < NUM_PHILOSOPHERS; fork++) {

			// If this Philosopher is holding this Fork, then print out
			// this Fork for this Philosopher.
			if (forks[fork] == phil) {
				printf("%d", fork);
			}
			// Otherwise, print a '-' character.
			else {
				printf("-");
			}
		}

		// Print the State that this Philosopher is currently in.
		switch (philosophers_to_print[phil].state) {
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
			fprintf(stderr, "Error: Philosopher %d has unknown state: %u", phil,
					philosophers_to_print[phil].state);

		}
	}

	// Formatting.
	printf("|\n");
}

void change_state(Philosopher *philosopher, State new_state) {

}

void pick_up_fork(Philosopher *philosopher, int fork) {

}

void put_down_fork(Philosopher *philosopher, int fork) {

}
