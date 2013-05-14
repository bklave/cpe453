#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void print_header() {
	puts(
			"|=============|=============|=============|=============|=============|");
	puts(
			"|      A      |      B      |      C      |      D      |      E      |");
	puts(
			"|=============|=============|=============|=============|=============|");
}

void print_footer() {
	puts(
			"|=============|=============|=============|=============|=============|");
}

void dawdle() {

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

}
