#include <string.h>

#include "../wrapper_functions.h"
#include "fs.h"
#include "sufs.h"
#include "path_utils.h"


int fs_errno;


char fs_cwd[] = ROOT_DIR;

static inline char* fs_getcwd() {
	return fs_cwd;
}


void* fs_open(const char* _path)
{
	char* cwd = fs_getcwd();
	char path[MAX_PATH_LEN];

	if (format_path(_path, cwd, path, MAX_PATH_LEN) == 0) {
		fs_errno = ENAMETOOLONG;
		return NULL;
	}

	if (!is_valid_path(path, "", MAX_FILENAME_LEN)) {
		fs_errno = ENAMETOOLONG;
		return NULL;
	}

	char* name = strrchr(path, PATH_SEPARATOR) + 1;
	if (!strcmp(name - 1, ROOT_DIR))
		name--;

	return sufs_open(path);
}

int fs_close(void* fd)
{
	return sufs_close(fd);
}

ssize_t fs_write(void* fd, void* data, uint64_t offset, size_t nbytes)
{
	if (nbytes > SSIZE_MAX) {
		fs_errno = EOVERFLOW;
		return -1;
	}

	return sufs_write(fd, data, offset, nbytes);
}

ssize_t fs_read(void* fd, void* buf, uint64_t offset, size_t nbytes)
{
	if (nbytes > SSIZE_MAX) {
		fs_errno = EOVERFLOW;
		return -1;
	}

	return sufs_read(fd, buf, offset, nbytes);
}

int fs_create(const char* _path)
{
	char* cwd = fs_getcwd();
	char path[MAX_PATH_LEN];

	if (format_path(_path, cwd, path, MAX_PATH_LEN) == 0) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	if (!is_valid_path(path, "", MAX_FILENAME_LEN)) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	return sufs_create(path);
}

int fs_unlink(const char* _path)
{
	char* cwd = fs_getcwd();
	char path[MAX_PATH_LEN];

	if (format_path(_path, cwd, path, MAX_PATH_LEN) == 0) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	if (!is_valid_path(path, "", MAX_FILENAME_LEN)) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	return sufs_unlink(path);
}

int fs_mkdir(const char* _path)
{
	char* cwd = fs_getcwd();
	char path[MAX_PATH_LEN];

	if (format_path(_path, cwd, path, MAX_PATH_LEN) == 0) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	if (!is_valid_path(path, "", MAX_FILENAME_LEN)) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	return sufs_mkdir(path);
}

int fs_rmdir(const char* _path)
{
	char* cwd = fs_getcwd();
	char path[MAX_PATH_LEN];

	if (format_path(_path, cwd, path, MAX_PATH_LEN) == 0) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	if (!is_valid_path(path, "", MAX_FILENAME_LEN)) {
		fs_errno = ENAMETOOLONG;
		return -1;
	}

	if (!strcmp(path, ROOT_DIR)) {
		fs_errno = EBUSY;
		return -1;
	}

	return sufs_rmdir(path);
}
