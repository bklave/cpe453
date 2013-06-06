/*
 * inode.c
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#include "inode.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void print_inode(Inode *inode) {
	int i = 0;

	printf("File inode:\n");
	printf("  unsigned short mode: %hu\n", inode->mode);
	printf("  unsigned short links: %hu\n", inode->links);
	printf("  unsigned short uid: %hu\n", inode->uid);
	printf("  unsigned short gid: %hu\n", inode->gid);
	printf("  unsigned long "
			" size: %lu\n", (long unsigned int) inode->size);
	printf("  unsigned long "
			" atime: %lu\n", (long unsigned int) inode->atime);
	printf("  unsigned long "
			" mtime: %lu\n", (long unsigned int) inode->mtime);
	printf("  unsigned long "
			" ctime: %lu\n\n", (long unsigned int) inode->ctime);

	printf("  Direct zones:\n");
	for (i = 0; i < DIRECT_ZONES; i++) {
		printf("			  zone[%d] = %d\n", i, inode->zone[i]);
	}

	printf("  unsigned long  indirect: %d\n", inode->indirect);
	printf("  unsigned long  double: %d\n", inode->two_indirect);
}

void print_directory(FILE *fp, Inode *inode, int blocksize) {
	long current_file_position = 0;
	int file_zone = 0;
	int num_directories = 0;
	int i = 0;
	DirectoryEntry *directoryEntries = NULL;

	// Read the current file position.
	if ((current_file_position = ftell(fp)) == -1) {
		perror("ftell");
		exit(-1);
	}

	// Seek to the directory's zone.
	file_zone = (inode->zone[0] * blocksize);
	if (fseek(fp, file_zone, SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}

	// Allocate memory for the DirectoryEntry objects we're about to copy.
	directoryEntries = malloc(inode->size);

	// Copy the DirectoryEntry objects from the FILE * and into memory.
	num_directories = inode->size / sizeof(DirectoryEntry);
	fread(directoryEntries, sizeof(DirectoryEntry), num_directories, fp);
	error_check_file_pointer(fp);

	// Print out the names of the directories.
	for (i = 0; i < num_directories; i++) {
		printf("Directory %d: %s\n", i, directoryEntries[i].name);
	}

	// Free the memory we allocated.
	free(directoryEntries);
}
