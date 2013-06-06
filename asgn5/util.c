/*
 * util.c
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#include "util.h"
#include <stdlib.h>
#include <assert.h>


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

void print_permissions_string(unsigned short mode) {
	char permissions_string[] = "----------";

	permissions_string[0] = (mode & DIRECTORY) ? 'd' : '-';
	permissions_string[1] = (mode & OWNER_READ_PERMISSION) ? 'r' : '-';
	permissions_string[2] = (mode & OWNER_WRITE_PERMISSION) ? 'w' : '-';
	permissions_string[3] = (mode & OWNER_EXECUTE_PERMISSION) ? 'x' : '-';
	permissions_string[4] = (mode & GROUP_READ_PERMISSION) ? 'r' : '-';
	permissions_string[5] = (mode & GROUP_WRITE_PERMISSION) ? 'w' : '-';
	permissions_string[6] = (mode & GROUP_EXECUTE_PERMISSION) ? 'x' : '-';
	permissions_string[7] = (mode & OTHER_READ_PERMISSION) ? 'r' : '-';
	permissions_string[8] = (mode & OTHER_WRITE_PERMISSION) ? 'w' : '-';
	permissions_string[9] = (mode & OTHER_EXECUTE_PERMISSION) ? 'x' : '-';

	printf("%s", permissions_string);
}
