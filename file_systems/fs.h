#pragma once

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>


#define IFMT   0xF000 	/* File type mask */
#define IFREG  0x8000  	/* Regular file */
#define IFDIR  0x4000  	/* Directory */
#define IFLNK  0xA000  	/* Symbolic link */
#define IFBLK  0x6000  	/* Block device */
#define IFCHR  0x2000  	/* Character device */
#define IFIFO  0x1000  	/* FIFO (named pipe) */
#define IFSOCK 0xC000  	/* Socket */

#define S_IRWXU 0x700   /* User (owner) read, write, execute */
#define S_IRUSR 0x400   /* User read permission */
#define S_IWUSR 0x200   /* User write permission */
#define S_IXUSR 0x100   /* User execute permission */
#define S_IRWXG 0x70   	/* Group read, write, execute */
#define S_IRGRP 0x40   	/* Group read permission */
#define S_IWGRP 0x20   	/* Group write permission */
#define S_IXGRP 0x10   	/* Group execute permission */
#define S_IRWXO 0x7   	/* Others read, write, execute */
#define S_IROTH 0x4   	/* Others read permission */
#define S_IWOTH 0x2   	/* Others write permission */
#define S_IXOTH 0x1   	/* Others execute permission */

#define UF_NODUMP  		0x1   	/* Do not dump */
#define UF_IMMUTABLE 	0x2 	/* Immutable */
#define UF_APPEND  		0x4   	/* Append-only */
#define UF_OPAQUE  		0x8   	/* Directory is opaque */
#define SF_ARCHIVED 	0x10 	/* File is archived */
#define SF_IMMUTABLE 	0x20 	/* File is immutable */
#define SF_APPEND 		0x40  	/* File is append-only */
#define SF_NOUNLINK 	0x80  	/* File cannot be unlinked */
#define SF_SNAPSHOT 	0x100 	/* File is a snapshot */
#define SF_SETTABLE 	0x200  	/* File flags are settable */


#define EPERM       	1    /* Operation not permitted */
#define ENOENT      	2    /* No such file or directory */
#define EIO         	5    /* Input/output error */
#define ENXIO       	6    /* No such device or address */
#define EBADF       	9    /* Bad file descriptor */
#define EAGAIN      	11   /* Resource temporarily unavailable */
#define ENOMEM      	12   /* Out of memory */
#define EACCES      	13   /* Permission denied */
#define EFAULT      	14   /* Bad address */
#define EBUSY       	16   /* Device or resource busy */
#define EEXIST      	17   /* File exists */
#define EXDEV       	18   /* Cross-device link */
#define ENODEV      	19   /* No such device */
#define ENOTDIR     	20   /* Not a directory */
#define EISDIR      	21   /* Is a directory */
#define EINVAL      	22   /* Invalid argument */
#define ENFILE      	23   /* File table overflow */
#define EMFILE      	24   /* Too many open files */
#define ENOTTY      	25   /* Inappropriate ioctl for device */
#define ETXTBSY     	26   /* Text file busy */
#define EFBIG       	27   /* File too large */
#define ENOSPC      	28   /* No space left on device */
#define ESPIPE      	29   /* Illegal seek */
#define EROFS       	30   /* Read-only file system */
#define EMLINK      	31   /* Too many links */
#define EPIPE       	32   /* Broken pipe */
#define EDOM        	33   /* Math argument out of domain of func */
#define ERANGE      	34   /* Math result not representable */
#define ENAMETOOLONG	36   /* File name too long */
#define ENOLCK      	37   /* No locks available */
#define ENOSYS      	38   /* Function not implemented */
#define ENOTEMPTY   	39   /* Directory not empty */
#define ELOOP       	40   /* Too many symbolic links encountered */
#define ENOMSG      	42   /* No message of desired type */
#define EIDRM       	43   /* Identifier removed */
#define ENOLINK     	67   /* Link has been severed */
#define EOVERFLOW   	75   /* Value too large for defined data type */
#define EILSEQ      	84   /* Illegal byte sequence */
#define EUSERS      	87   /* Too many users */
#define EDQUOT      	122  /* Disk quota exceeded */
#define ESTALE      	116  /* Stale file handle */



#define PATH_SEPARATOR 		'/'
#define ROOT_DIR 			"/"
#define MAX_FILENAME_LEN	255
#define MAX_PATH_LEN		4095


extern int fs_errno;


void* fs_open(const char* path);
int fs_close(void* fd);
ssize_t fs_write(void* fd, void* data, uint64_t offset, size_t nbytes);
ssize_t fs_read(void* fd, void* buf, uint64_t offset, size_t nbytes);

int fs_create(const char* path);
int fs_unlink(const char* path);
int fs_mkdir(const char* path);
int fs_rmdir(const char* path);
