#ifndef __UL_DATA_BUFFER_H__
#define __UL_DATA_BUFFER_H__

#include <stdint.h>
#include "libarch.h"

typedef struct {
    uint8_t *data;
    uint16_t size;
} ul_data_buffer_t;


ul_data_buffer_t* ul_data_buffer_create(void); 
ul_data_buffer_t* ul_data_buffer_create_with_size(uint16_t size); 
ul_data_buffer_t* ul_data_buffer_create_with_data(uint8_t *data, uint16_t size); 
void ul_data_buffer_destroy(ul_data_buffer_t* buffer);
#endif