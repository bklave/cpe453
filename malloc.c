#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "malloc.h"
#include "util.h"
#include "freelist.h"

void *malloc(size_t size) {
	Header *header = NULL;
	void *new_memory = NULL;
	long debug_flag = GetDebugFlag();
	char message[100];

	// Sanity check: size should be a positive number
	if (size <= 0) {
		return NULL ;
	}

	// Try to free some existing memory.
	if ((header = FindSomeAlreadyFreedMemoryFromFreeList(size)) != NULL ) {
		// If this succeeds, then do nothing. Use the returned Header *
		// as is.
	} else {
		// If we can't use already freed memory, then just make a new
		// Header *. freelist.c will handle the rest.
		if ((header = AllocateNewHeaderFromFreshMemory(size)) == NULL) {
			return NULL;
		}
	}

	// Pointer arithmetic: point the pointer to the actual data right
	// above the Header that we just allocated.
	new_memory = header + 1;

	// Sanity check: Memory addresses must be divisible by 16.
	if ((((long) new_memory) % kSixteen) != 0) {
//		fputs("\nERROR: memory address not divisible by 16!\n", stderr);
	}

	// Debug output
	if (debug_flag > 0) {
		snprintf(message, 100, "malloc(%d) =>"
				" (ptr=%p, size=%d)", size, new_memory, size);
		fputs(message, stderr);
	}

	return new_memory;
}

void free(void *ptr) {
	// Attempt to fetch the header for the given ptr.
	Header *header = FindSpecificHeader(ptr);

	if (header != NULL ) {
		// If we found the memory, mark it off as "is_free".
		header->is_free = true;
	} else {
//		fputs("couldn't free(); *ptr not in free list!\n", stderr);
	}
}

void *calloc(size_t num, size_t size) {
	char *malloc_result = NULL;
	const size_t bytes_to_allocate = num * size;
	int counter = 0;

	// Attempt to malloc() the memory requested.
	if ((malloc_result = malloc(bytes_to_allocate)) == NULL) {
		return NULL;
	}

	// Step through all the memory and initialize it all to 0.
	for (counter = 0; counter < bytes_to_allocate; ++counter) {
		*malloc_result = 0;
	}

	return malloc_result;
}

void *realloc(void *ptr, size_t size) {
	Header *header = NULL;

	if (ptr == NULL ) {
		return malloc(size);
	}

	// TODO: Don't forget to attempt to merge adjacent memory.

	if ((header = FindSpecificHeader(ptr)) != NULL) {
		if (size == header->size) {
			return ptr;
		} else if (size < header->size) {
			// TODO
		} else if (size > header->size) {
			// TODO
		}



	}


	// TODO: Finish realloc().
	return NULL;
}
