#ifndef __UTILITY_FLASH_H__
#define __UTILITY_FLASH_H__

typedef enum 
{
    PERIF_FLASH_FLASH_ON_BOARD,
    PERIF_FLASH_NOR_FLASH,
    PERIF_FLASH_RAM,
} PerifFlashType;

HAL_StatusTypeDef SAFE_ERASE_Sector(uint32_t Sector, uint8_t VoltageRange);
void PerifFlash_Init( void );
void PerifFlash_Write( PerifFlashType sel, uint32_t addr, uint32_t size, uint8_t* data );
void PerifFlash_Read( PerifFlashType sel, uint32_t addr, uint32_t size, uint8_t* buffer );
void PerifFlash_Erase( PerifFlashType Sel, uint32_t addr, int size );

#endif

