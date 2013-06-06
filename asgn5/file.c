/*
 * inode.c
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#include "file.h"
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
	long original_file_position = 0;
	int directory_zone = 0;
	int num_directories = 0;
	int i = 0;
	DirectoryEntry *directoryEntries = NULL;

	// Read the current file position.
	if ((original_file_position = ftell(fp)) == -1) {
		perror("ftell");
		exit(-1);
	}

	// Seek to the directory's zone.
	directory_zone = (inode->zone[0] * blocksize);
	if (fseek(fp, directory_zone, SEEK_SET)) {
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
		// TODO: Retrieve the file's inode here to print its
		// size and permissions.

		printf("PERMISSIONS, SIZE, %s\n", directoryEntries[i].name);
	}

	// Free the memory we allocated.
	free(directoryEntries);

	// Set the file position to go back to where it was before.
	if (fseek(fp, original_file_position, SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}
}

Inode get_inode(FILE *fp, Superblock *superblock, int inode_number) {
	Inode inode = { 0 };
	int original_file_position = 0;

	// Read the current file position.
	if ((original_file_position = ftell(fp)) == -1) {
		perror("ftell");
		exit(-1);
	}

	// Seek to the root inode in the system.
	if (fseek(fp, get_inode_index_for_number(superblock, 1), SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}

	// Read that root inode.
	fread(&inode, sizeof(Inode), 1, fp);
	error_check_file_pointer(fp);

	// Set the file position to go back to where it was before.
	if (fseek(fp, original_file_position, SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}

	return inode;
}
