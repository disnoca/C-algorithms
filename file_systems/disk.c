#include "disk.h"
#include "../wrapper_functions.h"


void disk_init(disk_t* disk, const char* filename, size_t sector_size, size_t block_size)
{
	disk->fp = Fopen(filename, "r+b");
	disk->sector_size = sector_size;
	disk->block_size = block_size;
	Fseek(disk->fp, 0, SEEK_END);
	disk->capacity = Ftell(disk->fp);
}

void disk_free(disk_t* disk)
{
	Fclose(disk->fp);
}

void disk_write(disk_t* disk, const void* data, uint64_t offset, uint64_t size)
{
	if (offset == 0)
		printf("Warning: Writing to sector 0\n");

	if (size == 0) {
		printf("Warning: Writing 0 bytes\n");
		return;
	}

	if (offset + size > disk->capacity)
		exit_with_error("Write exceeds disk capacity\n");

	Fseek(disk->fp, offset, SEEK_SET);
	Fwrite(data, 1, size, disk->fp);
}

void disk_read(disk_t* disk, void* buf, uint64_t offset, uint64_t size)
{
	if (offset == 0)
		printf("Warning: Reading from sector 0\n");

	if (size == 0) {
		printf("Warning: Reading 0 bytes\n");
		return;
	}
	
	if (offset + size > disk->capacity)
		exit_with_error("Read exceeds disk capacity\n");

	Fseek(disk->fp, offset, SEEK_SET);
	Fread(buf, 1, size, disk->fp);
}
