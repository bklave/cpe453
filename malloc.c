#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "malloc.h"
#include "util.h"
#include "freelist.h"

void *malloc(size_t size) {
	puts("called Girum's version of malloc()");

	Header *header = NULL;
	void *new_memory = NULL;

	// Try to free some existing memory.
	if ((header = FindSomeAlreadyFreedMemoryFromFreeList(size)) != NULL ) {
		// If this succeeds, then do nothing. Use the returned Header *
		// as is.
	} else {
		// If we can't use already freed memory, then just make a new
		// Header *. freelist.c will handle the rest.
		header = AllocateNewHeaderFromFreshMemory(sbrk(0), size);
	}

	// Pointer arithmetic: point the pointer to the actual data right
	// above the Header that we just allocated.
	new_memory = header + 1;

	// Sanity check: Memory addresses must be divisible by 16.
	if ((((long) new_memory) % kSixteen) != 0) {
		fprintf(stderr, "ERROR: memory address not divisible by 16!\n");
	}

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
		fprintf(stderr, "couldn't free(); *ptr not in free list!\n");
	}
}

void *calloc(size_t nmemb, size_t size) {
	puts("called Girum's version of calloc()");

	// TODO: Finish calloc().
	return 0;
}

void *realloc(void *ptr, size_t size) {
	puts("called Girum's version of realloc()");

	if (ptr == NULL ) {
		malloc(size);
	}

	// TODO: Finish realloc().
	return 0;
}
