// Don't include <stdlib.h>!
#include <stdio.h>
#include "malloc.h"
#include <unistd.h>

typedef struct {
	int header;
	char *data;
} Node;

Node *head = NULL;
long program_break = 0;
void *previous_program_break = NULL;
static const int kBytesToAllocate = 64000;

void *calloc(size_t nmemb, size_t size) {
	// TODO: Finish this function.
	puts("called Girum's version of calloc()");

	return 0;
}

void *malloc(size_t size) {
	puts("called Girum's version of malloc()");

	previous_program_break = sbrk(kBytesToAllocate);

	if (previous_program_break == (void *) -1) {
		perror("sbrk(2) failed");
	}

	Node new_node;



	return previous_program_break;
}

void free(void *ptr) {
	puts("called Girum's version of free()");
}

void *realloc(void *ptr, size_t size) {
	puts("called Girum's version of realloc()");

	if (ptr == NULL ) {
		malloc(size);
	}

	return 0;
}
