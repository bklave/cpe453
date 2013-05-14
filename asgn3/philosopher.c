#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "philosopher.h"

void print_status_line(Philosopher philosophers_to_print[], int forks[]) {
	int phil = 0, fork = 0;  // Counters.

	for (phil = 0; phil < NUM_PHILOSOPHERS; phil++) {
		// Formatting.
		printf("| ");

		// Print the forks that this Philosopher has in hand.
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
	philosopher->state = new_state;
}

void pick_up_fork(Philosopher *philosopher, int fork) {
	if (forks[fork] == -1) {
		forks[fork] = philosopher->id;
		printf("Philosopher %c picked up Fork %d.\n", philosopher->id + 'A',
				fork);
	} else {
		printf("Philosopher %c couldn't pick up Fork %d because "
				"Philosopher %c already had it.\n", philosopher->id + 'A', fork,
				forks[fork] + 'A');
	}
}

void put_down_fork(Philosopher *philosopher, int fork) {
	forks[fork] = -1;
	printf("Philosopher %c put down Fork %d.\n", philosopher->id + 'A', fork);
}
