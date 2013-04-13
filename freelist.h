/*
 * freelist.h
 *
 *  Created on: Apr 12, 2013
 *      Author: girum
 */

#include "util.h"

const static int kSixteen = 16;

struct HeaderStruct {
	size_t size;
	bool is_free;
	struct HeaderStruct *next;
};

typedef struct HeaderStruct Header;

/**
 * Used by malloc() to attempt to find existing freed memory.
 */
Header *FindSomeAlreadyFreedMemoryFromFreeList(size_t minimum_size);

/**
 * Used by malloc() to allocate a new Header * out of fresh Program Break space.
 */
Header *AllocateNewHeaderFromFreshMemory(void *previous_program_break,
		size_t size);

/**
 * Used by free() to delete a particular Header *
 */
Header *FindSpecificHeader(void *ptr);
