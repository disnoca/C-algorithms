#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


typedef struct disk_s {
	FILE* fp;
	uint64_t capacity;
	size_t sector_size, block_size;
} disk_t;


void disk_init(disk_t* disk, const char* filename, size_t sector_size, size_t block_size);
void disk_free(disk_t* disk);
void disk_write(disk_t* disk, const void* data, uint64_t offset, uint64_t size);
void disk_read(disk_t* disk, void* buf, uint64_t offset, uint64_t size);

static inline void disk_write_sectors(disk_t* disk, const void* data, uint64_t start_sector, uint64_t num_sectors) {
	disk_write(disk, data, start_sector * disk->sector_size, num_sectors * disk->sector_size);
}

static inline void disk_read_sectors(disk_t* disk, void* buf, uint64_t start_sector, uint64_t num_sectors) {
	disk_read(disk, buf, start_sector * disk->sector_size, num_sectors * disk->sector_size);
}

static inline void disk_write_blocks(disk_t* disk, const void* data, uint64_t start_block, uint64_t num_blocks) {
	disk_write(disk, data, start_block * disk->block_size, num_blocks * disk->block_size);
}

static inline void disk_read_blocks(disk_t* disk, void* buf, uint64_t start_block, uint64_t num_blocks) {
	disk_read(disk, buf, start_block * disk->block_size, num_blocks * disk->block_size);
}

static inline void disk_write_sector(disk_t* disk, const void* data, uint64_t sector) {
	disk_write_sectors(disk, data, sector, 1);
}

static inline void disk_read_sector(disk_t* disk, void* buf, uint64_t sector) {
	disk_read_sectors(disk, buf, sector, 1);
}

static inline void disk_write_block(disk_t* disk, const void* data, uint64_t block) {
	disk_write_blocks(disk, data, block, 1);
}

static inline void disk_read_block(disk_t* disk, void* buf, uint64_t block) {
	disk_read_blocks(disk, buf, block, 1);
}
