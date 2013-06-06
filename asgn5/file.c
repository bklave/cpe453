/*
 * inode.c
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_inode(Inode *inode) {
	int i = 0;

	printf("File inode:\n");
	printf("  unsigned short mode: 0x%x  (", inode->mode);
	print_permissions_string(inode->mode);
	printf(")\n");
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

void get_inode(Inode *inode_to_get, FILE *fp, Superblock *superblock,
		int inode_number) {
	// Seek to the inode specified, based on the inode_number given.
	if (fseek(fp, get_inode_index(superblock, inode_number), SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}

	// Read that inode into our struct.
	fread(inode_to_get, sizeof(Inode), 1, fp);
	error_check_file_pointer(fp);
}

void print_file(FILE *fp, Superblock *superblock, Inode *file_inode) {

}

void print_directory(FILE *fp, Superblock *superblock, Inode *directory_inode) {
	int first_zone_file_pointer = 0;
	int num_directories = 0;
	int i = 0;
	DirectoryEntry *directory_entries = NULL;
	Inode temp_inode = { 0 };

	// Point the file pointer to the directory of the first zone indexed by this inode.
	first_zone_file_pointer =
			(directory_inode->zone[0] * superblock->blocksize);
	if (fseek(fp, first_zone_file_pointer, SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}

	// Allocate memory for the DirectoryEntry objects we're about to copy.
	directory_entries = malloc(directory_inode->size);

	// Copy the DirectoryEntry objects from the FILE *, into memory.
	num_directories = directory_inode->size / sizeof(DirectoryEntry);
	fread(directory_entries, sizeof(DirectoryEntry), num_directories, fp);
	error_check_file_pointer(fp);

	// Print out the names of the DirectoryEntries.
	for (i = 0; i < num_directories; i++) {
		// Clear out temp_inode.
		memset(&temp_inode, 0, sizeof(Inode));

		/* "A DirectoryEntry with an inode_number of 0 is a file marked as
		 deleted. It is not a valid entry." */
		if (directory_entries[i].inode_number == 0) {
			continue;
		}

		// Retrieve this particular DirectoryEntry's inode.
		get_inode(&temp_inode, fp, superblock,
				directory_entries[i].inode_number);
		print_permissions_string(temp_inode.mode);

		// Print out the data for this DirectoryEntry.
		printf(", %d, %s\n", temp_inode.size, directory_entries[i].filename);
	}

	// Free the memory we allocated.
	free(directory_entries);
}

void find_file(FILE *fp, Superblock *superblock, char *path, int inode_number,
		bool verbose) {
	Inode inode = { 0 };

	// Get the inode for the inode_number given.
	get_inode(&inode, fp, superblock, inode_number);

	// If verbose, then print out the target path's inode data.
	if (verbose) {
		print_inode(&inode);
	}

	// Assume you found the inode for the correct path. Assume that this
	// inode is a directory. Print it out as a directory.
	printf("/:\n");
	print_directory(fp, superblock, &inode);
}

