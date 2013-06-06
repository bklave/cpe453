/*
 * superblock.h
 *
 *  Created on: Jun 6, 2013
 *      Author: girum
 */

#ifndef SUPERBLOCK_H_
#define SUPERBLOCK_H_

#include <stdint.h>

typedef struct {
	/* Minix Version 3 Superblock
	 * this structure found in fs/super.h
	 * in minix 3.1.1
	 */

	/* on disk. These ﬁelds and orientation are non–negotiable */
	uint32_t ninodes; /* number of inodes in this filesystem */
	uint16_t pad1; /* make things line up properly */
	int16_t i_blocks; /* # of blocks used by inode bit map */
	int16_t z_blocks; /* # of blocks used by zone bit map */
	uint16_t firstdata; /* number of first data zone */
	int16_t log_zone_size; /* log2 of blocks per zone */
	int16_t pad2; /* make things line up again */
	uint32_t max_file; /* maximum file size */
	uint32_t zones; /* number of zones on disk */
	int16_t magic; /* magic number */
	int16_t pad3; /* make things line up again */
	uint16_t blocksize; /* block size in bytes */
	uint8_t subversion; /* filesystem sub-version */
} Superblock;

void print_superblock(Superblock *superblock);

// Calculated "member datum."
int get_first_iblock(Superblock *superblock);
int get_zonesize(Superblock *superblock);
int get_ptrs_per_zone(Superblock *superblock);
int get_ino_per_block(Superblock *superblock);
int get_wrongended(Superblock *superblock);
int get_inode_index(Superblock *superblock, int inumber);

#endif /* SUPERBLOCK_H_ */
