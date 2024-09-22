# SUFS

An UFS based simple file system.

## Disk Layout

- Boot Block (1KB)
- Super Block (512B)
- Inode Bitmap
- Data Block Bitmap
- Inode Region
- Data Block Region

The first 1024 + 512 bytes of the disk are used by the Boot Block and Super Block.
The remaining areas are aligned to block boundaries, meaning that if the block size is greater than 512 bytes, there's padding between the Super Block and the Inode Bitmap.

## FS Parameters

The Block Size must be a power of 2 and be greater than the disk's sector size. It's minimum and maximum values are 512B and 64KB, respectively.

## Other Information

The Inode and Data Block number 0 are reserved and should not be used to store files. They should be set in their respective bitmaps when initializing the FS.
