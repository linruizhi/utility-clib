#ifndef __TECHNOLOGY_LIBARCH_LINUX_H__
#define __TECHNOLOGY_LIBARCH_LINUX_H__


#if UTILITY_SYSTEM_ARCH == 1

void* libc_calloc( int size );
void * libc_malloc( int size );
void libc_free( void * p );
void* libc_malloc_copy( uint8_t* src, uint16_t len );

#if UTILITY_SYSTEM_USE_OS
/* 架构头文件 */
#include <pthread.h>
#include <unistd.h>

typedef pthread_mutex_t  libcOsMutex_t;  //定义互斥量类型

libcOsMutex_t* libcOsMutexNew( void );
int libcOsMutexLock( libcOsMutex_t* mutex );
int libcOsMutexTryLock( libcOsMutex_t* mutex );
int libcOsMutexUnlock( libcOsMutex_t* mutex );
int libcOsMutexDelete( libcOsMutex_t* mutex );

#endif  //UTILITY_SYSTEM_USE_OS
#endif // UTILITY_SYSTEM_ARCH

#endif //_AWARE_TECHNOLOGY_LIBARCH_H
