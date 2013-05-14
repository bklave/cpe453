#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#include "philosopher.h"

void print_global_state() {
	int phil = 0, fork = 0;  // Counters.

	// Lock the global mutex thread.
	pthread_mutex_lock(&global_mutex_lock);

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
		switch (philosophers[phil].state) {
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
					philosophers[phil].state);
		}
	}

	// Formatting.
	printf("|\n");

	// Unlock the global mutex thread.
	pthread_mutex_unlock(&global_mutex_lock);
}

void change_state(Philosopher *philosopher, State new_state) {
	int left_fork = -1, right_fork = -1;

	left_fork = philosopher->assigned_left_fork;
	right_fork = philosopher->assigned_right_fork;

	switch (new_state) {
	case EATING:
		// If the Philosopher has his left_fork AND his right_fork, then
		// you can eat. Change the Philosophers' state and print status.
		if (forks[left_fork] == philosopher->id
				&& forks[right_fork] == philosopher->id) {
			philosopher->state = new_state;
			print_global_state();
		}
		// Otherwise, wait for the Philosopher who DOES have the left_fork to
		// drop it.
		else if (forks[left_fork] != philosopher->id) {
			// This parent thread will wait for each child thread in the order
			// of this array.
			int res = pthread_join(philosophers[forks[left_fork]].loop_thread,
					NULL );

			// Error check on the pthread_join() call.
			if (res == -1) {
				fprintf(stderr, "Child %i:	%s\n", forks[left_fork],
						strerror(errno));
				exit(-1);
			}

			// Once the other philosopher has dropped his fork
			philosopher->state = new_state;
			print_global_state();
		}
		// Otherwise, wait for the Philosopher who DOES have the left_fork to
		// drop it.
		else if (forks[right_fork] != philosopher->id) {
//			// This parent thread will wait for each child thread in the order
//			// of this array.
//			int res = pthread_join(philosophers[forks[right_fork]].thread,
//					NULL );
//
//			// Error check on the pthread_join() call.
//			if (res == -1) {
//				fprintf(stderr, "Child %i:	%s\n", forks[right_fork],
//						strerror(errno));
//				exit(-1);
//			}
		}

		break;
	case THINKING:
		// If the Philosopher isn't holding his left_fork AND isn't
		// holding his right_fork, then he can think.
		if (forks[left_fork] != philosopher->id
				&& forks[right_fork] != philosopher->id) {
			philosopher->state = new_state;
		} else {
			// Wait for the Philosopher who DOES have the fork to drop it.
		}

		break;
	case CHANGING:
		philosopher->state = new_state;
		break;
	default:
		perror("Unknown state");
		exit(-1);
	}

	// Print state.
	print_global_state();
}

void pick_up_fork(Philosopher *philosopher, int fork) {
	if (forks[fork] == -1) {

		// "Pick up" the Fork by giving it the correct Philosopher ID.
		forks[fork] = philosopher->id;
//		printf("Philosopher %c picked up Fork %d.\n", philosopher->id + 'A',
//				fork);
	} else {
//		printf("Philosopher %c couldn't pick up Fork %d because "
//				"Philosopher %c already had it.\n", philosopher->id + 'A', fork,
//				forks[fork] + 'A');
	}

	// Print state.
	print_global_state();
}

void put_down_fork(Philosopher *philosopher, int fork) {
	// "Put down" the Fork by giving it a Philosopher ID of -1.
	forks[fork] = -1;
//	printf("Philosopher %c put down Fork %d.\n", philosopher->id + 'A', fork);

	// Print state.
	print_global_state();
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
