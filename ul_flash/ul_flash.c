
#include "ul_flash.h"

/* 外部FLash地址 */
#define EXTERNAL_FLASH_ADDRESS_START    0x00000000
#define EXTERNAL_FLASH_ADDRESS_END      0x00800000
/* 内部FLash地址 */
#define INTERNAL_FLASH_ADDRESS_START    0x08000000
#define INTERNAL_FLASH_ADDRESS_END      0x08040000
/* 内部Ram地址 */
#define INTERNAL_RAM_ADDRESS_START      0x20000000
#define INTERNAL_RAM_ADDRESS_END        0x20020000
/* 外部Flash地址范围判断 */
#define _IS_EXTERNAL_FLASH( addr ) \
    ( ( EXTERNAL_FLASH_ADDRESS_START <= addr ) && ( EXTERNAL_FLASH_ADDRESS_END > addr ) )    \
/* 内部Flash地址范围判断 */
#define _IS_INTERNAL_FLASH( addr ) \
    ( ( INTERNAL_FLASH_ADDRESS_START <= addr ) && ( INTERNAL_FLASH_ADDRESS_END > addr ) )    \
/* 内部Ram地址范围判断 */    
#define _IS_INTERNAL_RAM( addr ) \
    ( ( INTERNAL_RAM_ADDRESS_START <= addr ) && ( INTERNAL_RAM_ADDRESS_END > addr ) )    \

#if UTILITY_SYSTEM_USE_OS
libcOsMutex_t   s_flash_mutex;
#define OS_FLASH_LOCK()     flash_valid_lock()
#define OS_FLASH_UNLOCK()   flash_valid_unlock()
static uint8_t flash_valid_unlock( void )
{
    if ( s_flash_mutex != NULL ) {
        xSemaphoreGive( s_flash_mutex );
    }
    
    return 0;
}

static uint8_t flash_valid_lock( void )
{
    if ( s_flash_mutex != NULL ) {
        BaseType_t err = pdFALSE;
        
        err = xSemaphoreTake( s_flash_mutex, 0xffffffff );

        if( err == pdTRUE )
        {
            return 0;
        }
    }
    
    return 1;
}
#else
#define OS_FLASH_LOCK()     flash_valid_lock()
#define OS_FLASH_UNLOCK()   flash_valid_unlock()
#endif

HAL_StatusTypeDef SAFE_ERASE_Sector(uint32_t Sector, uint8_t VoltageRange)
{
    FLASH_Erase_Sector( Sector, VoltageRange);
    
    if ( FLASH_WaitForLastOperation(1000) != HAL_OK) 
    {
        printf("FLASH WRITE ERROR IN ERASE!!! \r\n");
        return HAL_ERROR;
    }
    
    return HAL_OK;
}


void PerifFlash_Init( void )
{
#if UTILITY_SYSTEM_USE_OS
    s_flash_mutex = libcOsMutexNew();
    OS_FLASH_UNLOCK();
#endif
    sfud_init();
}

void PerifFlash_Write( PerifFlashType sel, uint32_t addr, uint32_t size, uint8_t* data )
{
    if ( sel == PERIF_FLASH_NOR_FLASH ) {
        OS_FLASH_LOCK();
        sfud_write( sfud_get_device( 0 ), addr, size, data );
        OS_FLASH_UNLOCK();
    }
}

void PerifFlash_Read( PerifFlashType sel, uint32_t addr, uint32_t size, uint8_t* buffer )
{   
    switch ( sel ) {
    case PERIF_FLASH_NOR_FLASH:
    {
        OS_FLASH_LOCK();
        sfud_read( sfud_get_device( 0 ), addr, size, buffer );
        OS_FLASH_UNLOCK();
    } break;
    case PERIF_FLASH_RAM:
    {
        OS_FLASH_LOCK();
        memmove( buffer, ( void* )addr, size );
        OS_FLASH_UNLOCK();
    } break;
    default:
        break;
    }
}


void PerifFlash_Erase( PerifFlashType sel, uint32_t addr, int size )
{
    if ( sel == PERIF_FLASH_NOR_FLASH ) {
        OS_FLASH_LOCK();
        sfud_erase( sfud_get_device( 0 ), addr, size );
        OS_FLASH_UNLOCK();
    }
}
