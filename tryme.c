/*
 * tryme.c
 *
 *  Created on: Apr 12, 2013
 *      Author: girum
 */

// Don't include <stdlib.h>!
#include <string.h>
#include <stdio.h>
#include "malloc.h"

int main(int argc, char *argv[]) {
	char *s;

	// Should call malloc() implicitly.
	s = strdup("Some string to strdup(). Should call malloc() implicitly.");

	puts(s);
	free(s);
	return 0;
}
