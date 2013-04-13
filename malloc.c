// Don't include <stdlib.h>!
#include <stdio.h>
#include "malloc.h"
#include <unistd.h>

static const int kBytesToAllocate = 64000;

typedef enum {
	false, true
} bool;

typedef struct {
	size_t size;
	bool is_free;
	struct Node *next;
} Header;

static Header *MoreSpace(size_t size) {
	Header *new_header;

	// Move the Program Break up by 64k bytes, plus the size of Header.
	void *previous_program_break = sbrk(sizeof(Header) + size);

	// sbrk(2) error check.
	if (previous_program_break == (char *) -1) {
		perror("sbrk(2) failed");
		return NULL ;
	}

	// Cast the beginning of the new memory to a Header *.
	new_header = (Header *) previous_program_break;

	// Initialize the new Header *.
	new_header->size = size;
	new_header->is_free = false;
	new_header->next = NULL;

	// Return a pointer to the new header.
	return new_header;
}

/**
 * Header of my free list.
 */
static Header *free_list_head = NULL;

void *malloc(size_t size) {
	puts("called Girum's version of malloc()");

	// Grab a new reference to head
	void *new_memory;

	if (free_list_head == NULL ) {
		free_list_head = MoreSpace(size);
	} else {
		free_list_head->next = MoreSpace(size);
	}

	return new_memory;
}

void free(void *ptr) {
	puts("called Girum's version of free()");

	Header *cursor = free_list_head;
	void *blob_pointer;

	// Traverse through the free list, looking for the correct
	while (cursor != NULL ) {
		blob_pointer = cursor + sizeof(Header);

		// If we found the blob pointer, then mark the memory as "free" and
		// return.
		if (ptr == blob_pointer) {
			cursor->is_free = true;
			return;
		}

		// Otherwise, keep iterating through the free list.
		cursor = cursor->next;
	}

	if (cursor == NULL ) {
		perror("couldn't free *ptr - either you called free(NULL) "
				"or the memory couldn't be found in the free list");
	}

}

void *calloc(size_t nmemb, size_t size) {
	// TODO: Finish this function.
	puts("called Girum's version of calloc()");

	return 0;
}

void *realloc(void *ptr, size_t size) {
	puts("called Girum's version of realloc()");

	if (ptr == NULL ) {
		malloc(size);
	}

	return 0;
}
