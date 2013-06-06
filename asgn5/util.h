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

/*
 * File type bitmasks.
 */
#define FILE_TYPE_MASK 0170000 // File type mask
#define REGULAR_FILE 0100000 // Regular Þle
#define DIRECTORY 0040000 // Directory
#define OWNER_READ_PERMISSION 0000400 // Owner read permission
#define OWNER_WRITE_PERMISSION 0000200 // Owner write permission
#define OWNER_EXECUTE_PERMISSION 0000100 // Owner execute permission
#define GROUP_READ_PERMISSION 0000040 // Group read permission
#define GROUP_WRITE_PERMISSION 0000020 // Group write permission
#define GROUP_EXECUTE_PERMISSION 0000010 // Group execute permission
#define OTHER_READ_PERMISSION 0000004 // Other read permission
#define OTHER_WRITE_PERMISSION 0000002 // Other write permission
#define OTHER_EXECUTE_PERMISSION 0000001 // Other execute permission
/*
 * END file type bitmasks.
 */

typedef enum {
	false, true
} bool;

void print_minls_usage();
void error_check_file_pointer(FILE *fp);

#endif /* UTIL_H_ */
