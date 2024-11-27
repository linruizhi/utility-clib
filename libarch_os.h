#ifndef __TECHNOLOGY_LIBARCH_OS_H__
#define __TECHNOLOGY_LIBARCH_OS_H__

#if UTILITY_SYSTEM_ARCH == 0

void* libc_calloc( int size );
void * libc_malloc( int size );
void libc_free( void * p );
void* libc_malloc_copy( uint8_t* src, uint16_t len );


#if UTILITY_SYSTEM_USE_OS
void libcOsDelayms( int ms );
#endif  //UTILITY_SYSTEM_USE_OS

#endif // UTILITY_SYSTEM_ARCH


#endif //_AWARE_TECHNOLOGY_LIBARCH_H
