/*
 * inode.h
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#ifndef INODE_H_
#define INODE_H_

#include <stdint.h>
#include <stdio.h>
#include "superblock.h"
#include "util.h"

#define DIRECT_ZONES 7
#define FILENAME_LENGTH 60

typedef struct {
	uint16_t mode; /* mode */
	uint16_t links; /* number of links */
	uint16_t uid;
	uint16_t gid;
	uint32_t size;
	int32_t atime;
	int32_t mtime;
	int32_t ctime;
	uint32_t zone[DIRECT_ZONES];
	uint32_t indirect;
	uint32_t two_indirect;
	uint32_t unused;
} Inode;

typedef struct {
	uint32_t inode_number; /* inode nubmer */
	unsigned char filename[FILENAME_LENGTH]; /* filename string */
} DirectoryEntry;

void find_file(FILE *fp, Superblock *superblock, int partition_sector_offset,
		char *requested_path, char *current_path, int inode_number,
		bool verbose);

#endif /* INODE_H_ */
