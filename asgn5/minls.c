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

int main(int argc, char *argv[]) {
	FILE *fp;
	int i = 0;

	for (i = 1; i < argc; i++) {

		// Flag processing. Check for a switch (leading "-").
		if (argv[i][0] == '-') {

			// Use the next character to decide what to do.
			switch (argv[i][1]) {
			case 'v':
				printf("Verbose flag called\n");
				break;
			case 'p':
				printf("Partition flag called\n");

				// Check for Subpartition flag.
				if (i + 3 < argc && argv[i + 2][1] == 's') {
					printf("Subpartition flag called\n");
				}

				break;
			}
		}
	}

	// Open the image file.
	if ((fp = fopen(argv[argc - 1], "r")) == NULL ) {
		perror("fopen");
		return -1;
	}

	// Do stuff.
	puts("Hello, World!");

	// Close the image file.
	if (fclose(fp) != 0) {
		perror("fclose");
		return -1;
	}
	return EXIT_SUCCESS;
}
