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
//		fputs("size <= 0, malloc() will return NULL", stderr);
		return NULL ;
	}

	// Try to free some existing memory.
	if ((header = FindSomeAlreadyFreedMemoryFromFreeList(size)) != NULL ) {
		// If this succeeds, then do nothing. Use the returned Header *
		// as is.
	} else {
		// If we can't use already freed memory, then just make a new
		// Header *. freelist.c will handle the rest.
//		snprintf(message, 100, "AllocateNewHeaderFromFreshMemory(%p)\n",
//				sbrk(0));
//		fputs(message, stderr);
		header = AllocateNewHeaderFromFreshMemory(size);
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

void *calloc(size_t nmemb, size_t size) {
	// TODO: Finish calloc().

	return 0;
}

void *realloc(void *ptr, size_t size) {
	if (ptr == NULL ) {
		return malloc(size);
	}

	// TODO: Finish realloc().
	return NULL;
}
