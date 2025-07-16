#include "libarch.h"

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

/* 对所有模块进行初始化，包括utility模组和以太网模块、ewlink*/
void utility_lib_Init( void )
{
    // DSAFE_PRINT( "Initialling UTILITYLib...\r\n" );
    
#if UTILITY_LIB_PARAMETER_MANAGEMENT
    Para_Init();
#endif // UTILITY_LIB_PARAMETER_MANAGEMENT

#if UTILITY_LIB_PROTOCOL_STACK
    Pstk_Init();
#endif //UTILITY_LIB_PROTOCOL_STACK
}


