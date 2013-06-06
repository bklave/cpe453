/*
 * util.h
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>

/**
 * Useful constants.
 */
#define PARTITION_TABLE_ADDRESS 0x1BE // location of the partition table
#define MINIX_PARTITION_TYPE 0x81 // partition type for Minix
#define BYTE_510 0x55 /* byte 510 of a boot sector with
						 a valid partition table */
#define BYTE_511 0xAA /* byte 511 of a boot sector with
						 a valid partition table */
#define MINIX_MAGIC_NUMBER 0x4D5A // the minix magic number
#define MINIX_MAGIC_NUMBER_REVERSED 0x5A4D /* minix magic number on
											  a byte-reversed Þlesystem */
#define INODE_SIZE 64 // size of an inode in bytes
#define DIRECTORY_ENTRY_SIZE 64 // size of a directory entry in bytes
#define SECTOR_SIZE 512 // size of a MINIX sector in bytes.
/*
 * End useful constants.
 */

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
#define ONE_KILOBYTE 1024 /* # of bytes in a Kilobyte */

typedef enum {
	false, true
} bool;

void print_minls_usage();
void error_check_file_pointer(FILE *fp);
void print_permissions_string(unsigned short mode);

#endif /* UTIL_H_ */
