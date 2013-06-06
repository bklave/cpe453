/*
 * inode.c
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#include "inode.h"

#include <stdio.h>

void print_inode(Inode *inode) {
	printf("File inode:\n");
	printf("  unsigned short mode: %hu\n", inode->mode);
	printf("  unsigned short links: %hu\n", inode->links);
	printf("  unsigned short uid: %hu\n", inode->uid);
	printf("  unsigned short gid: %hu\n", inode->gid);
	printf("  unsigned long "
			"size: %lu\n", (long unsigned int) inode->size);
	printf("  unsigned long "
			"atime: %lu\n", (long unsigned int) inode->atime);
	printf("  unsigned long "
			"mtime: %lu\n", (long unsigned int) inode->mtime);
	printf("  unsigned long "
			"ctime: %lu\n", (long unsigned int) inode->ctime);
}
