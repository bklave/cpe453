/*
 * util.h
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>

#define ONE_KILOBYTE 1024 /* # of bytes in a Kilobyte */

typedef enum {
	false, true
} bool;

void print_minls_usage();
void error_check_file_pointer(FILE *fp);
void print_permissions_string(unsigned short mode);

#endif /* UTIL_H_ */
