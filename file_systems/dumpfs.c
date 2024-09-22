#include <stdio.h>
#include <time.h>

#include "sufs.h"
#include "disk.h"


disk_t disk;
struct sufs_superblock sb;
uint8_t* block_buf;


static void read_superblock();


int main()
{
	disk_init(&disk, "disk.img", 512, 0);

	read_superblock();
	
	return 0;
}


static void read_superblock()
{
	disk_read_sector(&disk, &sb, 2);

	printf("magic      %-8X time        %lu\n", sb.sb_magic, sb.sb_time);
	printf("b_size     %-8u b_count     %-8u\n", sb.sb_block_size, sb.sb_block_count);
	printf("i_count    %-8u ib_count    %-8u db_count   %-7u\n", sb.sb_inode_count, sb.sb_iblock_count, sb.sb_dblock_count);
	printf("f_i_count  %-8u f_db_count  %-8u\n", sb.sb_free_inode_count, sb.sb_free_dblock_count);
	printf("i_map_sz   %-8u db_map_sz   %-8u\n", sb.sb_inode_map_bsize, sb.sb_dblock_map_bsize);
	printf("i_map_off  %-8u db_map_off  %-8u i_off      %-7u db_off     %-7u\n",
		sb.sb_inode_map_boff, sb.sb_dblock_map_boff, sb.sb_inodes_boff, sb.sb_dblocks_boff);
	printf("nindir     %-8u inopb       %-8u\n", sb.sb_nindir, sb.sb_inopb);
	printf("root_inum  %-8u maxfilesize %-8lu\n", sb.sb_roodir_inum, sb.sb_maxfilesize);
}
