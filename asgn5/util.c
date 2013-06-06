/*
 * util.c
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#include "util.h"

#include <stdlib.h>

void print_minls_usage() {
	printf("usage: ./nico_minls  [ -v ] [ -p num"
			" [ -s num ] ] imagefile [ path ]");
	printf("Options:");
	printf("	-p	 part    --- "
			"select partition for filesystem (default: none)");
	printf("	-s	 sub     --- "
			"select partition for filesystem (default: none)");
	printf("-h help --- print usage information and exit");
	printf("-v verbose --- increase verbosity level");
}

void error_check_file_pointer(FILE *fp) {
	// Check for errors on the fread().
	if (ferror(fp)) {
		perror("file error");
		exit(-1);
	}
}
