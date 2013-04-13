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

void *lowest_fresh_memory_address = NULL;

/**
 * Head node of my free list.
 */
Header *free_list_head_node = NULL;

/**
 * Increases the size of the free list by moving the Program Break up.
 */
void *IncreaseFreeListSize() {
	// Move the Program Break up by 64k bytes.
	void *temp_previous_program_break = sbrk(kBytesToReserve);

	// Update how much unallocated space I now have.
	unallocated_bytes_in_data_segment += kBytesToReserve;

	// sbrk(2) error check.
	if (temp_previous_program_break == (char *) -1) {
		perror("sbrk(2) failed");
		return NULL ;
	}

	return temp_previous_program_break;
}

Header *FindSomeAlreadyFreedMemoryFromFreeList(size_t minimum_size) {
	Header *cursor = free_list_head_node;

	// Traverse through the free list, looking for the correct blob data
	for (; cursor != NULL ; cursor = cursor->next) {

		// Check if this Header *'s memory is free and large enough
		if (cursor->is_free == true && cursor->size >= minimum_size) {
			return cursor;
		}
	}

	// If we couldn't find a suitable Header *, then return NULL.
	return NULL ;
}

Header *AllocateNewHeaderFromFreshMemory(size_t size) {
	Header *new_header = NULL;

	// The first time malloc() runs, we need to initialize our
	// lowest_fresh_memory_address. This is that time.
	if (free_list_head_node == NULL ) {
		lowest_fresh_memory_address = IncreaseFreeListSize();
		free_list_head_node = (Header *) lowest_fresh_memory_address;

		// Move lowest_fresh_memory_address pointer up to its correct
		// position
		lowest_fresh_memory_address += sizeof(Header);
		lowest_fresh_memory_address += size;

		// Fill up the free_list_head_node.
		free_list_head_node->size = size;
		free_list_head_node->is_free = false;
		free_list_head_node->next = NULL;

		// Reduce the size of unallocated_bytes_in_data_segment
		unallocated_bytes_in_data_segment -= sizeof(Header);
		unallocated_bytes_in_data_segment -= size;

		return free_list_head_node;
	} else {
		// If we *still* don't have enough room, then keep raising the
		// Program Break. Don't touch the previous_program_break though:
		// you don't move the program break *up* when allocating more
		// space for yourself.
		while (unallocated_bytes_in_data_segment < size) {
//			fputs("IncreaseFreeListSize()\n", stderr);
			IncreaseFreeListSize();
		}

		// Cast the beginning of the new memory to a Header *.
		new_header = (Header *) lowest_fresh_memory_address;

		// Point the previous_program_break to the end of that memory
		lowest_fresh_memory_address += sizeof(Header);
		lowest_fresh_memory_address += size;

		// Initialize the new Header.
		new_header->size = size;
		new_header->is_free = false;
		new_header->next = free_list_head_node;

		// Make this new node the new free_list_head_node.
		free_list_head_node = new_header;

		// Reduce the size of unallocated_bytes_in_data_segment
		unallocated_bytes_in_data_segment -= sizeof(Header);
		unallocated_bytes_in_data_segment -= size;

		return new_header;
	}
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

