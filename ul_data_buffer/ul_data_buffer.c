#include "ul_data_buffer.h"

ul_data_buffer_t* ul_data_buffer_create(void)
{
    ul_data_buffer_t* buffer = (ul_data_buffer_t*)libc_malloc(sizeof(ul_data_buffer_t));
    if (buffer)
    {
        buffer->data = NULL;
        buffer->size = 0;
    }
    return buffer;
}


ul_data_buffer_t* ul_data_buffer_create_with_size(uint16_t size)
{
    ul_data_buffer_t* buffer = ul_data_buffer_create();
    if (buffer)
    {
        buffer->data = libc_malloc(size);
        buffer->size = size;
        if (buffer->data == NULL)
        {
            libc_free(buffer);
            buffer = NULL;
        }
    }
    return buffer;
}


ul_data_buffer_t* ul_data_buffer_create_with_data(uint8_t *data, uint16_t size)
{
    ul_data_buffer_t* buffer = ul_data_buffer_create();
    if (buffer)
    {
        buffer->data = libc_malloc(size);
        buffer->size = size;
        if (buffer->data!= NULL)
        {
            memcpy(buffer->data, data, size);
        }
        else
        {
            libc_free(buffer);
            buffer = NULL;
        }
    }
    return buffer;
}


void ul_data_buffer_destroy(ul_data_buffer_t* data_buffer)
{
    if (data_buffer)
    {
        if (data_buffer->data != NULL && data_buffer->size)
        {
            libc_free(data_buffer->data);
            data_buffer->data = NULL;
            data_buffer->size = 0;
        }
        else
        {
            if( data_buffer->data!= NULL )
            {
                //size is 0,but data is not NULL, this is an error
                //添加打印
                printf("Error: data_buffer->data is not NULL, but data_buffer->size is 0!\n");
            }
            if( data_buffer->size!= 0 )
            {
                //size is not 0,but data is NULL, this is an error
                //添加打印
                printf("Error: data_buffer->size is not 0, but data_buffer->data is NULL!\n");
            }
        }
        libc_free(data_buffer);
        data_buffer = NULL;
    }
}