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

static void print_inode(Inode *inode) {
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

static void get_inode(Inode *inode_to_get, FILE *fp, Superblock *superblock,
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

/**
 * Returns a dynamic array of DirectoryEntries. Caller is responsible
 * for freeing these.
 */
static DirectoryEntry *get_directory_entries(FILE *fp, Superblock *superblock,
		Inode *directory_inode, int *num_directories) {
	DirectoryEntry *directory_entries = NULL;
	int first_zone_file_pointer = 0;

	// Point the file pointer to the DirectoryEntries inside the first
	// zone that is indexed by this inode.
	first_zone_file_pointer =
			(directory_inode->zone[0] * superblock->blocksize);
	if (fseek(fp, first_zone_file_pointer, SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}

	// Allocate memory for the DirectoryEntry objects we're about to copy.
	directory_entries = malloc(directory_inode->size);

	// Copy the DirectoryEntry objects from the FILE *, into memory.
	*num_directories = directory_inode->size / sizeof(DirectoryEntry);
	fread(directory_entries, sizeof(DirectoryEntry), *num_directories, fp);
	error_check_file_pointer(fp);

	return directory_entries;
}

static void print_file(FILE *fp, Superblock *superblock, int inode_number,
		char *filename) {
	Inode temp_inode = { 0 };

	// Retrieve this particular DirectoryEntry's inode.
	get_inode(&temp_inode, fp, superblock, inode_number);
	print_permissions_string(temp_inode.mode);

	// Print out the data for this DirectoryEntry.
	printf(", %d, %s\n", temp_inode.size, filename);
}

static void print_directory(FILE *fp, Superblock *superblock,
		Inode *directory_inode) {
	DirectoryEntry *directory_entries = NULL;
	Inode temp_inode = { 0 };
	int num_directories = 0;
	int i = 0;

	// Get the DirectoryEntries for this directory.
	directory_entries = get_directory_entries(fp, superblock, directory_inode,
			&num_directories);

	// Print out the names of the DirectoryEntries.
	for (i = 0; i < num_directories; i++) {
		// Clear out temp_inode.
		memset(&temp_inode, 0, sizeof(Inode));

		/* "A DirectoryEntry with an inode_number of 0 is a file marked as
		 deleted. It is not a valid entry." */
		if (directory_entries[i].inode_number == 0) {
			continue;
		}

		print_file(fp, superblock, directory_entries[i].inode_number,
				directory_entries[i].filename);

//		// Retrieve this particular DirectoryEntry's inode.
//		get_inode(&temp_inode, fp, superblock,
//				directory_entries[i].inode_number);
//		print_permissions_string(temp_inode.mode);
//
//		// Print out the data for this DirectoryEntry.
//		printf(", %d, %s\n", temp_inode.size, directory_entries[i].filename);
	}

	// Free the DirectoryEntries we allocated.
	free(directory_entries);
}

/*
 * Recursive function. Bulk of the logic is here.
 */
void find_file(FILE *fp, Superblock *superblock, char *requested_path,
		char *current_path, int inode_number, bool verbose) {
	Inode inode = { 0 };
	DirectoryEntry *directory_entries = NULL;
	int num_directories = 0;
	int i = 0;
	char *parsed_token = NULL;
	char new_path[60] = { '\0' };

	// Get the inode for the inode_number given.
	get_inode(&inode, fp, superblock, inode_number);

	// Base case: this is a FILE. In this case, you either found the
	// file that was requested or you didn't.
	if ((inode.mode & DIRECTORY) == 0) {
		// If the path that we're on right now is the one that was
		// requested by the user, then we're good. Print the file.
		if (strcmp(requested_path, current_path) == 0) {
			if (verbose) {
				print_inode(&inode);
			}
			printf("%s:\n", requested_path);

			print_file(fp, superblock, inode_number, requested_path);

			return;
		}
		// Otherwise, we stopped on a file and it wasn't the one the user
		// requested. Exit failure here.
		else {
			fprintf(stderr,
					"Couldn't find file with current path %s: %s requested\n",
					current_path, requested_path);
			return;
		}
	}
	// Otherwise, this is a DIRECTORY.
	else {
		// If the path that we're on right now is the one that was
		// requested by the user, then we're good. Print the directory
		// and return.
		if (strcmp(requested_path, current_path) == 0) {
			if (verbose) {
				print_inode(&inode);
			}

			printf("%s:\n", requested_path);
			print_directory(fp, superblock, &inode);
			return;
		}
		// Otherwise, traverse down this directory.
		else {
			printf("Traversing down directory %s to try and match %s...\n",
					current_path, requested_path);

			// Truncate your requested_path by one directory.
			parsed_token = strtok(requested_path, "/");

			// Grab each of the DirectoryEntries in this directtory.
			directory_entries = get_directory_entries(fp, superblock, &inode,
					&num_directories);

			// Find the DirectoryEntry that points in the directory
			// direction that we want to go.
			for (i = 0; i < num_directories; i++) {
				if (strcmp(parsed_token, directory_entries[i].filename) == 0) {
					printf("Found filename match: %s\n", parsed_token);

					// Add the parsed token to the current_path.
					strcpy(new_path, current_path);
					strcat(new_path, parsed_token);

					// Make a recursive call with the udpated path.
					find_file(fp, superblock, requested_path, new_path,
							directory_entries[i].inode_number, verbose);
					return;
				}

			}

			// If you process each DirectoryEntry without finding the one
			// that corresponds to the requested filename, then return
			// failure.
			fprintf(stderr, "No file/directory %s found within directory %s\n",
					requested_path, current_path);
			return;
		}
	}

}

