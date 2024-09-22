#include <string.h>
#include <assert.h>

#include "disk.h"
#include "fs.h"
#include "sufs.h"
#include "../wrapper_functions.h"


static void fs_is_empty(disk_t* disk)
{
	// Check that FS is empty
	uint8_t* buf = Malloc(disk->block_size);
	struct sufs_superblock* sb = (struct sufs_superblock*)buf;

	disk_read_sector(disk, buf, SUFS_SUPERBLOCK_OFFSET / disk->sector_size);
	assert(sb->sb_free_inode_count == sb->sb_inode_count - 1);
	assert(sb->sb_free_dblock_count == sb->sb_dblock_count - 1);

	uint32_t dblock_map_boff = sb->sb_dblock_map_boff;
	disk_read_block(disk, buf, sb->sb_inode_map_boff);
	assert(buf[0] == 0x03);
	for (uint32_t i = 1; i < sb->sb_block_size; i++)
		assert(buf[i] == 0x00);

	disk_read_block(disk, buf, dblock_map_boff);
	assert(buf[0] == 0x03);
	for (uint32_t i = 1; i < sb->sb_block_size; i++)
		assert(buf[i] == 0x00);

	Free(buf);
}


static void test_create_delete()
{
	// Successfully create files and directories
	assert(!fs_create("/file1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_create("/file2") || !printf("errno: %d\n", fs_errno));
	assert(!fs_mkdir("/dir1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_mkdir("/dir2") || !printf("errno: %d\n", fs_errno));
	assert(!fs_create("/dir1/file1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_create("/dir1/file2") || !printf("errno: %d\n", fs_errno));
	assert(!fs_mkdir("/dir1/dir1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_create("/dir1/dir1/file1") || !printf("errno: %d\n", fs_errno));

	// Unsuccessfully create duplicates
	assert(fs_create("/file1") < 0);
	assert(fs_errno == EEXIST);
	assert(fs_mkdir("/file1") < 0);
	assert(fs_errno == EEXIST);
	assert(fs_create("/dir1") < 0);
	assert(fs_errno == EEXIST);

	// Unsuccessfully create files and directories with invalid paths
	assert(fs_create("/dir3/file1") < 0);
	assert(fs_errno == ENOENT);
	assert(fs_create("/dir1/file1/file1") < 0);
	assert(fs_errno == ENOTDIR);

	// Successfully open and close files
	void* fd1, *fd2, *fd3, *fd4;
	assert((fd1 = fs_open("/file1")) != NULL || !printf("errno: %d\n", fs_errno));
	assert((fd2 = fs_open("/dir1/file2")) != NULL || !printf("errno: %d\n", fs_errno));
	assert((fd3 = fs_open("/dir1/dir1/file1")) != NULL || !printf("errno: %d\n", fs_errno));
	assert((fd4 = fs_open("/dir1/dir1")) != NULL || !printf("errno: %d\n", fs_errno));
	assert(!fs_close(fd1) || !printf("errno: %d\n", fs_errno));
	assert(!fs_close(fd2) || !printf("errno: %d\n", fs_errno));
	assert(!fs_close(fd3) || !printf("errno: %d\n", fs_errno));
	assert(!fs_close(fd4) || !printf("errno: %d\n", fs_errno));

	assert(fs_open("/file3") == NULL);
	assert(fs_errno == ENOENT);

	// Unsuccessfully remove files and directories
	assert(fs_rmdir("/file1") < 0);
	assert(fs_errno == ENOTDIR);
	assert(fs_unlink("/dir1") < 0);
	assert(fs_errno == EISDIR);

	// Successfully remove files and directories
	assert(!fs_unlink("/file1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_unlink("/file2") || !printf("errno: %d\n", fs_errno));
	assert(!fs_rmdir("/dir2") || !printf("errno: %d\n", fs_errno));

	assert(fs_open("/file1") == NULL);
	assert(fs_errno == ENOENT);

	// Unsuccessfully remove non-existent files and directories
	assert(fs_unlink("/file1") < 0);
	assert(fs_errno == ENOENT);
	assert(fs_rmdir("/dir2") < 0);
	assert(fs_errno == ENOENT);

	// Unsuccessfully remove non-empty directories
	assert(fs_rmdir("/dir1") < 0);
	assert(fs_errno == ENOTEMPTY);
	assert(fs_rmdir("/dir1/dir1") < 0);
	assert(fs_errno == ENOTEMPTY);

	assert(!fs_unlink("/dir1/dir1/file1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_rmdir("/dir1/dir1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_unlink("/dir1/file2") || !printf("errno: %d\n", fs_errno));

	assert(fs_rmdir("/dir1") < 0);
	assert(fs_errno == ENOTEMPTY);

	assert(!fs_unlink("/dir1/file1") || !printf("errno: %d\n", fs_errno));
	assert(!fs_rmdir("/dir1") || !printf("errno: %d\n", fs_errno));
}


const char lorem_ipsum[] =	
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
	"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
	"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. "
	"Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
	"Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. "
	"Proin vel nulla nunc. Sed elementum lectus at sem posuere tempus ac vel quam. Sed id urna at magna posuere euismod id sed leo. "
	"In lacinia pharetra massa, a semper ipsum vulputate et. Etiam tortor erat, venenatis sit amet nulla nec, posuere luctus magna. "
	"Integer maximus augue sem, a eleifend justo maximus sed. Ut scelerisque neque et ultrices luctus. "
	"Mauris lobortis dui eget elit dapibus, et suscipit sem accumsan. Cras nec pharetra dolor. Phasellus nec tortor ipsum. "
	"Aliquam id eleifend libero. Integer faucibus et purus a sollicitudin. "
	"Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. "
	"Nullam ligula tortor, semper eu gravida eget, auctor vitae massa. In facilisis erat sem, sed imperdiet leo iaculis ut. "
	"Nunc maximus nisl feugiat orci consequat dictum. Aenean at vulputate dolor. Morbi hendrerit erat turpis, a pharetra est semper sed. "
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent congue nibh nec neque condimentum dignissim. "
	"In hac habitasse platea dictumst. Curabitur suscipit venenatis sapien ac pellentesque. Suspendisse sit amet rutrum ex. "
	"Phasellus blandit dui nec ipsum rhoncus, quis gravida justo posuere. Vivamus lectus lacus, congue ac ex vel, placerat mattis nisl. "
	"Pellentesque nibh est, pretium et est in, tempus egestas ipsum. Sed scelerisque nibh ante, nec scelerisque leo commodo pellentesque. "
	"Etiam eget hendrerit leo. "
	"Pellentesque tortor lectus, posuere vel felis tincidunt, ullamcorper gravida dui. Praesent gravida arcu vitae porttitor ultricies. "
	"Pellentesque finibus sem dapibus dui malesuada, sed tempor magna hendrerit. Vestibulum egestas dolor sed gravida dignissim. "
	"Aenean pellentesque est est, non aliquam velit gravida ut. Cras tortor nisi, hendrerit commodo lacinia nec, elementum non leo. "
	"Proin lobortis urna placerat libero euismod, a tempor nisi auctor. Suspendisse potenti. Sed pellentesque id orci vel interdum. "
	"Phasellus sit amet turpis at ligula cursus blandit. Curabitur a ipsum pellentesque, fermentum magna vitae, dignissim sem. "
	"In hac habitasse platea dictumst."
	"Curabitur quis dignissim sem. Nunc magna magna, volutpat vel laoreet sit amet, accumsan nec nulla. "
	"Nullam ultricies quam velit, sit amet vestibulum est placerat et. Praesent at lacus luctus, lobortis massa vel, interdum lacus. "
	"Donec semper lacinia lorem. In aliquam mi nec blandit imperdiet. "
	"Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Proin vulputate lacinia magna convallis dictum. "
	"Maecenas enim purus, volutpat at nunc eu, consequat sollicitudin urna. Quisque consequat neque lorem, sed aliquam quam mattis et. "
	"Neque porro quisquam est qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit...";

static void test_read_write()
{
	char* buf = Malloc(sizeof(lorem_ipsum));
	ssize_t ret;

	assert(!fs_create("/lorem ipsum.txt"));
	void* fd = fs_open("/lorem ipsum.txt");
	assert(fd != NULL);

	// Aligned singled block read/write
	assert((ret = fs_write(fd, (void*)lorem_ipsum, 0, 1024) == 1024) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	assert(fs_read(fd, buf, 0, 2000) == 1024 || !printf("errno: %d\n", fs_errno));
	assert(!memcmp(buf, lorem_ipsum, 1024));

	// Unaligned singled block read/write
	assert((ret = fs_write(fd, (void*)lorem_ipsum, 219, 768) == 768) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	assert(fs_read(fd, buf, 219, 768) == 768 || !printf("errno: %d\n", fs_errno));
	assert(!memcmp(buf, lorem_ipsum, 768));

	// Aligned multi-block read/write
	assert((ret = fs_write(fd, (void*)lorem_ipsum, 1024, 3072) == 3072) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	assert(fs_read(fd, buf, 1024, 4000) == 3072 || !printf("errno: %d\n", fs_errno));
	assert(!memcmp(buf, lorem_ipsum, 3072));

	// Unaligned multi-block read/write
	assert((ret = fs_write(fd, (void*)lorem_ipsum, 999, sizeof(lorem_ipsum)) == sizeof(lorem_ipsum)) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	assert(fs_read(fd, buf, 999, sizeof(lorem_ipsum)) == sizeof(lorem_ipsum) || !printf("errno: %d\n", fs_errno));
	assert(!memcmp(buf, lorem_ipsum, sizeof(lorem_ipsum)));

	// Combined write/single read
	ssize_t part = sizeof(lorem_ipsum) / 3;
	size_t offset = 0;
	assert((ret = fs_write(fd, (void*)lorem_ipsum, offset, part) == part) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	offset += part;
	assert((ret = fs_write(fd, (void*)(lorem_ipsum + offset), offset, part) == part) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	offset += part;
	assert((ret = fs_write(fd, (void*)(lorem_ipsum + offset), offset, part) == part) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	offset += part;
	assert((ret = fs_write(fd, (void*)(lorem_ipsum + offset), offset, sizeof(lorem_ipsum) - offset) == (ssize_t)(sizeof(lorem_ipsum) - offset)) ||
			!printf("ret: %ld errno: %d\n", ret, fs_errno));
	assert(fs_read(fd, buf, 0, sizeof(lorem_ipsum)) == sizeof(lorem_ipsum) || !printf("errno: %d\n", fs_errno));
	assert(!memcmp(buf, lorem_ipsum, sizeof(lorem_ipsum)));

	assert(!fs_close(fd));
	assert(!fs_unlink("/lorem ipsum.txt"));
	Free(buf);
}


int main()
{
	system("./newfs");
	disk_t disk;
	disk_init(&disk, "disk.img", 512, 0);

	sufs_mount(&disk);

	test_create_delete();
	fs_is_empty(&disk);
	test_read_write(&disk);
	fs_is_empty(&disk);

	sufs_unmount();
	disk_free(&disk);

	assert(malloc_calls == free_calls);
	
	printf("All tests passed!\n");
	return 0;
}
