#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "malloc.h"
#include "util.h"
#include "freelist.h"

void *malloc(size_t size) {
	puts("called Girum's version of malloc()");

	// TODO: Handle the case where the size passed in is greater than 64k bytes.

	Header *header = NULL;
	void *new_memory = NULL;

	// Try to free some existing memory.
	if ((header = FindSomeAlreadyFreedMemoryFromFreeList(size)) != NULL ) {
		// If this succeeds, then do nothing. Use the returned Header * as is.
	} else {
		// If we can't use already freed memory, then just make a new Header *.
		// freelist.c will handle the rest.
		header = AllocateNewHeaderFromFreshMemory(sbrk(0), size);
	}

	// Pointer arithmetic: point the pointer to the actual data right above
	// the Header that we just allocated.
	new_memory = header + 1;

	// TODO: Perform this check when sbrk(2) is called, and fix it then.
	if ((((long) new_memory) % 16) != 0) {
		fprintf(stderr, "memory address is not divisible by 16!\n");
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
		fprintf(stderr, "couldn't free(); *ptr not found in free list!\n");
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
