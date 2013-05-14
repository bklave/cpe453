/*
 * util.c
 *
 *  Created on: Apr 12, 2013
 *      Author: girum
 */
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

static const char *kDebugFlag = "DEBUG_MALLOC";

long GetDebugFlag() {
	long debug_flag = 0;

	// Grab the string version.
	const char *debug_flag_string = getenv(kDebugFlag);

	// If the string is there, cast it to a long.
	if (debug_flag_string != NULL ) {
		debug_flag = strtol(debug_flag_string, NULL, 10);
	} else {
//		fputs("\"DEBUG_MALLOC\" flag not set.", stderr);
	}

	return debug_flag;
}

