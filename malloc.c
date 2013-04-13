// Don't include <stdlib.h>!
#include <stdio.h>
#include "malloc.h"
#include <unistd.h>

static const int kBytesToAllocate = 64000;

typedef enum {
	false, true
} bool;

struct HeaderStruct {
	size_t size;
	bool is_free;
	struct HeaderStruct *next;
};

typedef struct HeaderStruct Header;

Header *SbrkSomeMoreSpace(size_t size) {
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

/**
 * Attempts to fetch an existing Header *.
 */
Header *FindSpecificHeader(void *ptr) {
	Header *cursor = free_list_head;
	void *blob_pointer = NULL;

	// Traverse through the free list, looking for the correct blob data
	while (cursor != NULL ) {
		blob_pointer = cursor + 1;

		// Check if this Header * corresponds to the pointer we want.
		if (ptr == blob_pointer) {
			return cursor;
		}

		// Otherwise, keep iterating through the free list.
		cursor = cursor->next;
	}

	// If we couldn't find the Header * for the given ptr, then return NULL.
	return NULL ;
}

/**
 * Attempts to find some free memory as large as requested.
 */
Header *FindSomeFreeMemoryFromFreeList(size_t minimum_size) {
	Header *cursor = free_list_head;

	// Traverse through the free list, looking for the correct blob data
	while (cursor != NULL ) {

		// Check if this Header *'s memory is free and large enough
		if (cursor->is_free == true && cursor->size >= minimum_size) {
			return cursor;
		}

		// Otherwise, keep iterating through the free list.
		cursor = cursor->next;
	}

	// If we couldn't find a suitable Header *, then return NULL.
	return NULL ;
}

void *malloc(size_t size) {
	puts("called Girum's version of malloc()");

	Header *header = NULL;
	void *new_memory = NULL;

	if (free_list_head == NULL ) {
		// If this is the first time running malloc(), then allocate 64k bytes
		// of memory.
		free_list_head = SbrkSomeMoreSpace(kBytesToAllocate);
		header = free_list_head;
	} else if ((header = FindSomeFreeMemoryFromFreeList(size))) {
		// If it's NOT the first time running malloc(), then attempt to find
		// some existing free memory.

	}

	new_memory = header + 1;

	return new_memory;
}

void free(void *ptr) {
	puts("called Girum's version of free()");

	// Attempt to fetch the header for the given ptr.
	Header *header = FindSpecificHeader(ptr);

	if (header != NULL ) {
		// If we found the memory, mark it off as "is_free".
		header->is_free = true;
	} else {
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
