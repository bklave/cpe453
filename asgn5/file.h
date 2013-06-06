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
	uint32_t inode; /* inode nubmer */
	unsigned char name[FILENAME_LENGTH]; /* filename string */
} DirectoryEntry;

void print_inode(Inode *file_inode);
void get_inode(Inode *inode_to_get, FILE *fp, Superblock *superblock,
		int inode_number);
void print_file(FILE *fp, Superblock *superblock, Inode *file_inode);
void print_directory(FILE *fp, Superblock *superblock, Inode *directory_inode);
void find_file(FILE *fp, Superblock *superblock, char *path, int inode_number,
		bool verbose);

#endif /* INODE_H_ */
