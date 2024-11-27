#include "libarch.h"

#if UTILITY_SYSTEM_USE_OS
#if UTILITY_SYSTEM_ARCH == 1
/*
*********************************************************************************************************
*	函 数 名: libc_malloc
*	功能说明: 申请内存
*	形    参: size
*	返 回 值: 地址
*********************************************************************************************************
*/
void* libc_malloc( int size )
{
    void* tmp;
	
    tmp = malloc( size );
    
    return tmp;
}

/*
*********************************************************************************************************
*	函 数 名: libc_calloc
*	功能说明: 申请内存，并初始化为0
*	形    参: size
*	返 回 值: 地址
*********************************************************************************************************
*/
void* libc_calloc( int size )
{
    void* tmp;
	
    tmp = libc_malloc( size );
    if ( tmp != NULL)
    {
        memset(tmp, 0, size);
    }
    
    return tmp;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void* libc_malloc_copy( uint8_t* src, uint16_t len )
{
    uint8_t *des;

    des = libc_malloc( len );
    if( des != NULL )
    {
        memcpy( des, src, len );
    }

    return des;
}


/*
*********************************************************************************************************
*	函 数 名: libc_free
*	功能说明: 释放内存
*	形    参: 地址
*	返 回 值: 无
*********************************************************************************************************
*/
void libc_free( void * p )
{
    if ( p == NULL ) {
        return;
    }
    
	free( p );
    p=NULL;
}

/*
*********************************************************************************************************
*	函 数 名: libcOsDelayms
*	功能说明: 任务延迟，进入阻塞态
*	形    参: 延迟时间
*	返 回 值: 无
*********************************************************************************************************
*/
void libcOsDelayms( int ms )
{
    usleep(ms*1000);
}


/* 申请互斥亮 */
libcOsMutex_t* libcOsMutexNew(void)
{
    int ret;
    libcOsMutex_t* mutex;
    mutex = malloc(sizeof(pthread_mutex_t));
    if ( mutex == NULL ) {
        printf("rb->mutex is NULL\n");
        return mutex;
    }

    ret = pthread_mutex_init(mutex,NULL);
    if( ret != 0 )
    {
        perror("pthread_mutex_init error\n");
        libc_free(mutex);
        mutex = NULL;
    }
    return mutex;
}


/*
*********************************************************************************************************
*	函 数 名: libcOsRecursiveMutexLock
*	功能说明: 互斥量获取，阻塞
*	形    参: 信号量句柄
*	返 回 值: 操作结果
*********************************************************************************************************
*/
int libcOsMutexLock( libcOsMutex_t* mutex )
{
    int err;
    err = pthread_mutex_lock(mutex);
    if( err != 0)
    {
        perror("mutex_lock");
    }
    return err;
}

/*
*********************************************************************************************************
*	函 数 名: libcOsMutexTryLock
*	功能说明: 互斥量获取，阻塞
*	形    参: 信号量句柄
*	返 回 值: 操作结果
*********************************************************************************************************
*/
int libcOsMutexTryLock( libcOsMutex_t* mutex )
{
    int err;
    err = pthread_mutex_trylock( mutex );
    return err;
}

/*
*********************************************************************************************************
*	函 数 名: libcOsMutexUnlock
*	功能说明: 互斥量获取，阻塞
*	形    参: 信号量句柄
*	返 回 值: 操作结果
*********************************************************************************************************
*/
int libcOsMutexUnlock( libcOsMutex_t* mutex )
{
    int err;
    if( mutex == NULL ) return -1;
    err = pthread_mutex_unlock(mutex);
    if( err != 0)
    {
        perror("mutex_unlock");
    }
    return err;
}

/*
*********************************************************************************************************
*	函 数 名: libcOsMutexDelete
*	功能说明: 互斥量删除
*	形    参: 信号量句柄
*	返 回 值: 无
*********************************************************************************************************
*/
int libcOsMutexDelete( libcOsMutex_t* mutex )
{
    int err;
    err = pthread_mutex_destroy(mutex);
    if( err != 0)
    {
        perror("mutex_unlock");
    }
    return err;
}

#endif //UTILITY_SYSTEM_ARCH
#endif //UTILITY_SYSTEM_USE_OS



