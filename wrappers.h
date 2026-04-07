#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


// Terminate program on an error by default
#if !defined(WRAPPERS_ERROR_ACTION_TERMINATE) || !defined(WRAPPERS_ERROR_ACTION_LOG)
#define WRAPPERS_ERROR_ACTION_TERMINATE
#endif

#ifdef WRAPPERS_ERROR_ACTION_TERMINATE
#define wrappers_error_func wrappers_exit_with_error
#elif defined(WRAPPERS_ERROR_ACTION_LOG)
#define wrappers_error_func wrappers_log_error
#else
#error "Invalid error action. Define either WRAPPERS_ERROR_ACTION_TERMINATE or WRAPPERS_ERROR_ACTION_LOG."
#endif


// Do not force inline by default
#ifdef WRAPPERS_FORCE_INLINE
#define WRAPPERS_INLINE static inline __attribute__((always_inline))
#else
#define WRAPPERS_INLINE static inline
#endif


#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define WRAPPERS_NORETURN _Noreturn
#else
#define WRAPPERS_NORETURN /* nothing */
#endif

#ifdef __GNUC__
#define wrappers_likely(x)       __builtin_expect(!!(x), 1)
#define wrappers_unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define wrappers_likely(x)       (x)
#define wrappers_unlikely(x)     (x)
#endif

/* ---------- Macro Definitions ---------- */

#define Wlog_error(format, ...) wrappers_log_error(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define Wexit_with_error(format, ...) wrappers_exit_with_error(__FILE__, __LINE__, format, ##__VA_ARGS__)

#ifdef WRAPPERS_DISABLE

#define Malloc malloc
#define Calloc calloc
#define Free free
#define Realloc realloc

#define Fopen fopen
#define Fclose fclose
#define Fwrite fwrite
#define Fread fread
#define Fseek fseek
#define Ftell ftell

#define Ftruncate ftruncate
#define Close close

#define Shm_open shm_open
#define Mmap mmap
#define Munmap munmap

#define Pthread_attr_init pthread_attr_init
#define Pthread_attr_destroy pthread_attr_destroy
#define Pthread_attr_setstack pthread_attr_setstack
#define Pthread_attr_getstack pthread_attr_getstack
#define Pthread_create pthread_create
#define Pthread_join pthread_join
#define Pthread_mutex_lock pthread_mutex_lock
#define Pthread_mutex_unlock pthread_mutex_unlock
#define Pthread_getattr_np wrappers_pthread_getattr_np

#define Sysconf sysconf

#else

#define Malloc(size) wrappers_malloc(size, __FILE__, __LINE__)
#define Calloc(size, nmemb) wrappers_calloc(size, nmemb, __FILE__, __LINE__)
#define Free free
#define Realloc(ptr, size) wrappers_realloc(ptr, size, __FILE__, __LINE__)

#define Fopen(path, mode) wrappers_fopen(path, mode, __FILE__, __LINE__)
#define Fclose(fp) wrappers_fclose(fp, __FILE__, __LINE__)
#define Fwrite(ptr, size, nmemb, stream) wrappers_fwrite(ptr, size, nmemb, stream, __FILE__, __LINE__)
#define Fread(ptr, size, nmemb, stream) wrappers_fread(ptr, size, nmemb, stream, __FILE__, __LINE__)
#define Fseek(stream, offset, whence) wrappers_fseek(stream, offset, whence, __FILE__, __LINE__)
#define Ftell(stream) wrappers_ftell(stream, __FILE__, __LINE__)

#define Ftruncate(fd, len) wrappers_ftruncate(fd, len, __FILE__, __LINE__)
#define Close(fd) wrappers_close(fd, __FILE__, __LINE__)

#define Shm_open(name, flags, mode) wrappers_shm_open(name, flags, mode, __FILE__, __LINE__)
#define Mmap(addr, len, prot, flags, fd, offset) wrappers_mmap(addr, len, prot, flags, fd, offset, __FILE__, __LINE__)
#define Munmap(addr, len) wrappers_munmap(addr, len, __FILE__, __LINE__)

#define Pthread_attr_init(attr) wrappers_pthread_attr_init(attr, __FILE__, __LINE__)
#define Pthread_attr_destroy(attr) wrappers_pthread_attr_destroy(attr, __FILE__, __LINE__)
//#define Pthread_getattr_np(thread, attr) wrappers_pthread_getattr_np(thread, attr, __FILE__, __LINE__)
#define Pthread_attr_setstack(attr, addr, size) wrappers_pthread_attr_setstack(attr, addr, size, __FILE__, __LINE__)
#define Pthread_attr_getstack(attr, addr, size) wrappers_pthread_attr_getstack(attr, addr, size, __FILE__, __LINE__)
#define Pthread_create(thread, attr, start_routine, arg) wrappers_pthread_create(thread, attr, start_routine, arg, __FILE__, __LINE__)
#define Pthread_join(thread, retval) wrappers_pthread_join(thread, retval, __FILE__, __LINE__)
#define Pthread_mutex_lock(mutex) wrappers_pthread_mutex_lock(mutex)
#define Pthread_mutex_unlock(mutex) wrappers_pthread_mutex_unlock(mutex)

#define Sysconf(name) wrappers_sysconf(name, __FILE__, __LINE__)

#endif /* WRAPPERS_DISABLE */


/* ---------- 0. Error Handling ---------- */

WRAPPERS_INLINE
void wrappers_log_error(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);
	if (file)
    	fprintf(stderr, "%s:%d: error: ", file, line);
    vfprintf(stderr, format, args);
	if (format[strlen(format) - 1] != '\n')
   		fprintf(stderr, "\n");
    va_end(args);
}

#define wrappers_exit_with_error(file, line, format, ...) do { \
    wrappers_log_error(file, line, format, ##__VA_ARGS__); \
    exit(EXIT_FAILURE); \
} while(0)


/* ---------- 1. Memory Management ---------- */

WRAPPERS_INLINE
void* wrappers_malloc(size_t size, const char* file, int line)
{
	void* ptr = malloc(size);
	if(wrappers_unlikely(!ptr))
		wrappers_error_func(file, line, "malloc error %d: %s", errno, strerror(errno));
	return ptr;
}

WRAPPERS_INLINE
void* wrappers_calloc(size_t size, size_t nmemb, const char* file, int line)
{
	void* ptr = calloc(nmemb, size);
	if(wrappers_unlikely(!ptr))
		wrappers_error_func(file, line, "calloc error %d: %s", errno, strerror(errno));
	return ptr;
}

WRAPPERS_INLINE
void* wrappers_realloc(void* ptr, size_t size, const char* file, int line)
{
	void* new_ptr = realloc(ptr, size);
	if(wrappers_unlikely(!new_ptr))
		wrappers_error_func(file, line, "realloc error %d: %s", errno, strerror(errno));
	return new_ptr;
}


/* ---------- 4. File Operations ---------- */

/* ---------- 4.1 Standard I/O ---------- */

WRAPPERS_INLINE
FILE* wrappers_fopen(const char* path, const char* mode, const char* file, int line)
{
    FILE* fp = fopen(path, mode);
    if(wrappers_unlikely(!fp))
        wrappers_error_func(file, line, "fopen error %d: %s", errno, strerror(errno));
    return fp;
}

WRAPPERS_INLINE
void wrappers_fclose(FILE* fp, const char* file, int line)
{
    if(wrappers_unlikely(fclose(fp) == EOF))
        wrappers_error_func(file, line, "fclose error %d: %s", errno, strerror(errno));
}

WRAPPERS_INLINE
size_t wrappers_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream, const char* file, int line)
{
    size_t ret = fwrite(ptr, size, nmemb, stream);
    if(wrappers_unlikely(ret < nmemb))
        wrappers_error_func(file, line, "fwrite error %d: %s", errno, strerror(errno));
    return ret;
}

WRAPPERS_INLINE
size_t wrappers_fread(void* ptr, size_t size, size_t nmemb, FILE* stream, const char* file, int line)
{
    size_t ret = fread(ptr, size, nmemb, stream);
    if(wrappers_unlikely(ret < nmemb && ferror(stream)))
        wrappers_error_func(file, line, "fread error %d: %s", errno, strerror(errno));
    return ret;
}

WRAPPERS_INLINE
void wrappers_fseek(FILE* stream, long offset, int whence, const char* file, int line)
{
    if(wrappers_unlikely(fseek(stream, offset, whence) == -1))
        wrappers_error_func(file, line, "fseek error %d: %s", errno, strerror(errno));
}

WRAPPERS_INLINE
long wrappers_ftell(FILE* stream, const char* file, int line)
{
    long ret = ftell(stream);
    if(wrappers_unlikely(ret == -1))
        wrappers_error_func(file, line, "ftell error %d: %s", errno, strerror(errno));
    return ret;
}

/* ---------- 4.2 POSIX I/O ---------- */

WRAPPERS_INLINE
int wrappers_ftruncate(int fd, off_t len, const char* file, int line)
{
	int ret = ftruncate(fd, len);
	if(wrappers_unlikely(ret == -1))
		wrappers_error_func(file, line, "ftruncate error %d: %s", errno, strerror(errno));
	return ret;
}

WRAPPERS_INLINE
int wrappers_close(int fd, const char* file, int line)
{
	int ret = close(fd);
	if(wrappers_unlikely(ret == -1))
		wrappers_error_func(file, line, "close error %d: %s", errno, strerror(errno));
	return ret;
}

/* ---------- 4.3 Memory-mapped I/O ---------- */

WRAPPERS_INLINE
int wrappers_shm_open(const char* name, int flags, mode_t mode, const char* file, int line)
{
	int ret = shm_open(name, flags, mode);
	if(wrappers_unlikely(ret == -1))
		wrappers_error_func(file, line, "shm_open error %d: %s", errno, strerror(errno));
	return ret;
}

WRAPPERS_INLINE
void* wrappers_mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset, const char* file, int line)
{
	void* ret = mmap(addr, len, prot, flags, fd, offset);
	if(wrappers_unlikely(ret == MAP_FAILED))
		wrappers_error_func(file, line, "mmap error %d: %s", errno, strerror(errno));
	return ret;
}

WRAPPERS_INLINE
int wrappers_munmap(void* addr, size_t len, const char* file, int line)
{
	int ret = munmap(addr, len);
	if(wrappers_unlikely(ret == -1))
		wrappers_error_func(file, line, "munmap error %d: %s", errno, strerror(errno));
	return ret;
}


/* ---------- 3. Thread Management ---------- */

WRAPPERS_INLINE
int wrappers_pthread_attr_init(pthread_attr_t* attr, const char* file, int line)
{
	int ret = pthread_attr_init(attr);
	if(wrappers_unlikely(ret))
		wrappers_error_func(file, line, "pthread_attr_init error %d: %s", ret, strerror(ret));
	return ret;
}

WRAPPERS_INLINE
int wrappers_pthread_attr_destroy(pthread_attr_t* attr, const char* file, int line)
{
	int ret = pthread_attr_destroy(attr);
	if(wrappers_unlikely(ret))
		wrappers_error_func(file, line, "pthread_attr_destroy error %d: %s", ret, strerror(ret));
	return ret;
}

WRAPPERS_INLINE
int wrappers_pthread_attr_setstack(pthread_attr_t* attr, void* addr, size_t size, const char* file, int line)
{
	int ret = pthread_attr_setstack(attr, addr, size);
	if(wrappers_unlikely(ret))
		wrappers_error_func(file, line, "pthread_attr_setstack error %d: %s", ret, strerror(ret));
	return ret;
}

WRAPPERS_INLINE
int wrappers_pthread_attr_getstack(const pthread_attr_t* attr, void** addr, size_t* size, const char* file, int line)
{
	int ret = pthread_attr_getstack(attr, addr, size);
	if(wrappers_unlikely(ret))
		wrappers_error_func(file, line, "pthread_attr_getstack error %d: %s", ret, strerror(ret));
	return ret;
}

WRAPPERS_INLINE
int wrappers_pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void*), void* arg, const char* file, int line)
{
	int ret = pthread_create(thread, attr, start_routine, arg);
	if(wrappers_unlikely(ret))
		wrappers_error_func(file, line, "pthread_create error %d: %s", ret, strerror(ret));
	return ret;
}

WRAPPERS_INLINE
int wrappers_pthread_join(pthread_t thread, void** retval, const char* file, int line)
{
	int ret = pthread_join(thread, retval);
	if(wrappers_unlikely(ret))
		wrappers_error_func(file, line, "pthread_join error %d: %s", ret, strerror(ret));
	return ret;
}

WRAPPERS_INLINE
void wrappers_pthread_mutex_lock(pthread_mutex_t* mutex)
{
	int ret = pthread_mutex_lock(mutex);
	if(wrappers_unlikely(ret))
		wrappers_error_func(NULL, 0, "pthread_mutex_lock error %d: %s", ret, strerror(ret));
}

WRAPPERS_INLINE
void wrappers_pthread_mutex_unlock(pthread_mutex_t* mutex)
{
	int ret = pthread_mutex_unlock(mutex);
	if(wrappers_unlikely(ret))
		wrappers_error_func(NULL, 0, "pthread_mutex_unlock error %d: %s", ret, strerror(ret));
}

#ifdef _GNU_SOURCE
#ifndef Pthread_getattr_np
#define Pthread_getattr_np(thread, attr) wrappers_pthread_getattr_np(thread, attr, __FILE__, __LINE__)

WRAPPERS_INLINE
int wrappers_pthread_getattr_np(pthread_t thread, pthread_attr_t* attr, const char* file, int line)
{
	int ret = pthread_getattr_np(thread, attr);
	if(wrappers_unlikely(ret))
		wrappers_error_func(file, line, "pthread_getattr_np error %d: %s", ret, strerror(ret));
	return ret;
}
#endif
#endif


/* ---------- 4. System Information ---------- */

WRAPPERS_INLINE
long wrappers_sysconf(int name, const char* file, int line)
{
	long ret = sysconf(name);
	if(wrappers_unlikely(ret == -1 && errno == EINVAL))
		wrappers_error_func(file, line, "sysconf error %d: %s", errno, strerror(errno));
	return ret;
}