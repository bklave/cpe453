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
#include "superblock.h"
#include "file.h"
#include "util.h"

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
/*
 * End useful constants.
 */

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

	// If verbose, then print the superblock's data.
	if (verbose) {
		print_superblock(superblock);
	}

	return fp;
}

int main(int argc, char *argv[]) {
	FILE *fp = NULL;
	int i = 0;
	char *image_filename = NULL, *path = NULL;
	bool verbose = false, partition = false, subpartition = false;
	Superblock superblock = { 0 };

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
	}
	// Otherwise, the user didn't specify a path after all.
	else {
		image_filename = argv[argc - 1];
		path = "\\";
	}

	// Initalize the file stream and the superblock.
	fp = initalize(fp, &superblock, image_filename, verbose);

	// Find and print the correct file.
	find_file(fp, &superblock, path, 1, verbose);

	// Close the image file.
	if (fclose(fp) != 0) {
		perror("fclose");
		exit(-1);
	}
	return EXIT_SUCCESS;
}
