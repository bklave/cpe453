/*
 * inode.c
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#include "inode.h"

#include <stdio.h>

void print_inode(Inode *inode) {
	int i = 0;

	printf("File inode:\n");
	printf("  unsigned short mode: %hu\n", inode->mode);
	printf("  unsigned short links: %hu\n", inode->links);
	printf("  unsigned short uid: %hu\n", inode->uid);
	printf("  unsigned short gid: %hu\n", inode->gid);
	printf("  unsigned long "
			" size: %lu\n", (long unsigned int) inode->size);
	printf("  unsigned long "
			" atime: %lu\n", (long unsigned int) inode->atime);
	printf("  unsigned long "
			" mtime: %lu\n", (long unsigned int) inode->mtime);
	printf("  unsigned long "
			" ctime: %lu\n\n", (long unsigned int) inode->ctime);

	printf("  Direct zones:\n");
	for (i = 0; i < DIRECT_ZONES; i++) {
		printf("			  zone[%d] = %d\n", i, inode->zone[i]);
	}

	printf("  unsigned long  indirect: %d\n", inode->indirect);
	printf("  unsigned long  double: %d\n", inode->two_indirect);
}

void print_directory(Inode *inode) {
//	DirectoryEntry directory_entry = { 0 };
//
//
//
//	// Read the directory entry from the first zone?
//	memcpy(&directory_entry, inode->zone[0], sizeof(DirectoryEntry));
//
//	// Print out the filename?
//	printf("Directory entry filename: %s", directory_entry.name);
}
