/*
 ============================================================================
 Name        : minls.c
 Author      : Girum Ibssa
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "superblock.h"
#include "file.h"
#include "util.h"

static FILE *initalize(FILE *fp, Superblock *superblock, char *filename,
		bool verbose) {
	// Open the image file.
	if ((fp = fopen(filename, "r")) == NULL ) {
		perror("fopen");
		exit(-1);
	}

	// Seek to the superblock.
	if (fseek(fp, ONE_KILOBYTE, SEEK_SET)) {
		perror("fseek");
		exit(-1);
	}

	// Read the superblock.
	fread(superblock, sizeof(Superblock), 1, fp);
	error_check_file_pointer(fp);

	// Sanity check on the superblock's magic number.
	if (superblock->magic != MINIX_MAGIC_NUMBER) {
		fprintf(stderr, "Bad magic number. (0x%x)\n", superblock->magic);
		fprintf(stderr, "This doesn't look like a MINIX filesystem.\n");
		exit(-1);
	}

	// If verbose, then print the superblock's data.
	if (verbose) {
		print_superblock(superblock);
	}

	return fp;
}

int main(int argc, char *argv[]) {
	FILE *fp = NULL;
	int i = 0;
	char *image_filename = NULL;
	char *path = NULL;
	bool verbose = false, partition = false, subpartition = false;
	Superblock superblock = { 0 };
	char *new_path = NULL;

	for (i = 1; i < argc; i++) {

		// Flag processing. Check for a switch (leading "-").
		if (argv[i][0] == '-') {

			// Use the next character to decide what to do.
			switch (argv[i][1]) {
			case 'v':
				verbose = true;
				break;
			case 'p':
				printf("Partition!\n");
				partition = true;

				// Check for Subpartition flag.
				if ((argc > (i + 3)) && argv[i + 2][1] == 's') {
					printf("Subpartition!\n");
					subpartition = true;
				}

				break;
			}
		}
	}

	/* If there are three or more arguments and both the second to last
	 * and last arguments aren't flags, then the user specified a path. */
	if (argc > 2 && argv[argc - 2][0] != '-' && argv[argc - 1][0] != '-') {
		image_filename = argv[argc - 2];
		path = argv[argc - 1];

		/* "Paths that do not include a leading �/� are processed
		 * relative to the root directory" */
		if (path[0] != '/') {
			new_path = malloc(sizeof(char) * strlen(path) + 2);
			strcpy(new_path, "/");
			strcat(new_path, path);
			path = new_path;
		}
	}
	// Otherwise, the user didn't specify a path after all.
	else {
		image_filename = argv[argc - 1];
		path = "/";
	}

	// Open and initalize the image. Initialize the superblock.
	fp = initalize(fp, &superblock, image_filename, verbose);

	// Find and print the correct file/directory.
	find_file(fp, &superblock, path, "/", 1, verbose);

	// If you had to malloc a new_path, then free it.
	if (new_path != NULL ) {
		free(new_path);
	}

	// Close the image.
	if (fclose(fp) != 0) {
		perror("fclose");
		exit(-1);
	}
	return EXIT_SUCCESS;
}
