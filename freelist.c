/*
 * freelist.c
 *
 *  Created on: Apr 12, 2013
 *      Author: girum
 */

#include <stdio.h>
#include "freelist.h"

static const int kBytesToReserve = 64000;

/**
 * The number of bytes that I have free in my data segment
 */
long unallocated_bytes_in_data_segment = 0;

/**
 * Head node of my free list.
 */
Header *free_list_head_node = NULL;

/**
 * Increases the size of the free list by moving the Program Break up.
 */
void *IncreaseFreeListSize() {
	// Move the Program Break up by 64k bytes.
	void *previous_program_break = sbrk(kBytesToReserve);

	// Update how much unallocated space I now have.
	unallocated_bytes_in_data_segment += kBytesToReserve;

	// sbrk(2) error check.
	if (previous_program_break == (char *) -1) {
		perror("sbrk(2) failed");
		return NULL ;
	}

	return previous_program_break;
}

Header *FindSomeAlreadyFreedMemoryFromFreeList(size_t minimum_size) {
//	Header *cursor = free_list_head_node;
	Header *cursor = NULL;
	char message[100];

	// Traverse through the free list, looking for the correct blob data
	for (cursor = free_list_head_node; cursor != NULL ;
			cursor = cursor->next) {

		snprintf(message, 100, "cursor = %p, cursor->next = %p\n", cursor,
				cursor->next);
		fputs(message, stderr);

		// Check if this Header *'s memory is free and large enough
		if (cursor->is_free == true && cursor->size >= minimum_size) {
			return cursor;
		}

		// Otherwise, keep iterating through the free list.
//		fputs("cursor = cursor->next\n\n", stderr);
	}

	// If we couldn't find a suitable Header *, then return NULL.
	return NULL ;
}

Header *AllocateNewHeaderFromFreshMemory(void *previous_program_break,
		size_t size) {
	Header *new_header = NULL;

	// If we don't have enough room to increase our size, then raise the
	// Program Break once.
	if (unallocated_bytes_in_data_segment < size) {
		previous_program_break = IncreaseFreeListSize();
	}

	// If we *still* don't have enough room, then keep raising the
	// Program Break. Keep the previous_program_break put from the first
	// time though.
	while (unallocated_bytes_in_data_segment < size) {
		IncreaseFreeListSize();
	}

	// Cast the beginning of the new memory to a Header *.
	new_header = (Header *) previous_program_break;

	// Initialize the new Header *.
	new_header->size = size;
	new_header->is_free = false;
	new_header->next = free_list_head_node;

	// Make this new node the new free_list_head_node.
	free_list_head_node = new_header;

	// Reduce the size of unallocated_bytes_in_data_segment
	unallocated_bytes_in_data_segment -= sizeof(Header);
	unallocated_bytes_in_data_segment -= size;

	// Return a pointer to the new header.
	return new_header;
}

Header *FindSpecificHeader(void *ptr) {
	Header *cursor = free_list_head_node;
	void *blob_pointer = NULL;

	// Traverse through the free list, looking for the correct blob data
	while (cursor != NULL ) {
		// Point the void * to the memory right after the Header.
		blob_pointer = cursor + 1;

		// Check if this Header * corresponds to the pointer we want.
		if (ptr == blob_pointer) {
			return cursor;
		}

		// Otherwise, keep iterating through the free list.
		cursor = cursor->next;
	}

	// If we couldn't find the Header * for the given ptr, then return
	// NULL.
	return NULL ;
}

