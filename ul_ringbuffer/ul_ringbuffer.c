/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 17:17:33
 * @LastEditors: linruizhi1124@outlook.com linruizhi1124@outlook.com
 * @LastEditTime: 2024-11-28 15:41:18
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#include <stdio.h>
#include <stdint.h>
#include "ul_error.h"
#include "ul_ringbuffer.h"


/**
 * @description: 创建ringbuffer
 * @param {ul_rb_attr_t} attr
 * @return {*}
 */
ul_ringbuffer_t* ul_ringbuffer_create( ul_rb_attr_t attr )
{
    ul_ringbuffer_t* rb = NULL;
    uint32_t size;

    if( attr.element_size && attr.capacity )
    {
        if( attr.buff == NULL )
        {
            attr.buff = libc_malloc( attr.element_size*attr.capacity );
        }

        if( attr.buff )
        {
            rb = libc_malloc( sizeof( ul_ringbuffer_t ) );
            if( rb == NULL )
            {
                libc_free( attr.buff );
            }
        }

        if( rb )
        {
            rb->buff = attr.buff;
            rb->buff = attr.capacity;
            rb->element_size = attr.element_size;
            rb->head = rb->tail = rb->buff;
            rb->buff_size = attr.capacity*attr.element_size;
        }
    }

    return rb;
}

/**
 * @description: 对已存在的ringbuffer进行初始化
 * @param {ul_ringbuffer_t} *rb
 * @param {ul_rb_attr_t} attr
 * @return {*}
 */
int ul_ringbuffer_init( ul_ringbuffer_t *rb, ul_rb_attr_t attr )
{
    int ret = UL_INVAILD;

    if( rb && attr.element_size && attr.capacity )
    {
        rb->buff_size = attr.element_size*attr.capacity;

        if( attr.buff == NULL )
        {
            attr.buff = libc_malloc( rb->buff_size );
        }

        if( attr.buff )
        {
            rb->buff = attr.buff;
            rb->capacity = attr.capacity;
            rb->element_size = attr.element_size;
            rb->head = rb->tail = rb->buff;

            ret = UL_SUCCESS;
        }
        else
        {
            ret = UL_NOMEMARY;
        }
    }

    return ret;
}

/**
 * @function: 
 * @description: 获取已存在的元素数量
 * @param {ul_ringbuffer_t} *rb
 * @return {*}
 */
int ul_ringbuffer_get_element_number( ul_ringbuffer_t *rb )
{
    int num;

    if( !(rb && rb->buff) ){
        return UL_INVAILD;
    }
    
    if ( rb->head == rb->tail )
    {
        num = 0;
    }
    else if( rb->head < rb->tail )
    {
        num = (char*)rb->tail - rb->head;
        num = num / rb->element_size;
    }
    else{
        num = rb->buff_size - ((char*)rb->head - rb->tail);
        num = num / rb->element_size;
    }

    return num;
}

/**
 * @description: 获取可写入元素的数量
 * @param {ul_ringbuffer_t} *rb
 * @return {*}
 */
int ul_ringbuffer_get_free_element_number( ul_ringbuffer_t *rb )
{
    int num;
    
    num = rb->capacity - ul_ringbuffer_get_element_number( rb );

    return num;
}

/**
 * @description: 删除一个元素
 * @param {ul_ringbuffer_t} *rb
 * @return {*}
 */
int ul_ringbuffer_del_element( ul_ringbuffer_t *rb )
{
    int ret,delSz;

    if( !(rb && rb->buff) ){
        return UL_INVAILD;
    }
    
    if( rb->head == rb->tail )
    {
        ret = UL_DATA_ERROR;
    }
    else
    {
        delSz = rb->element_size;
        
        if( (char*)rb->head + delSz >= rb->buff_size + (char*)rb->buff )
        {
            rb->head = (char*)rb->head + delSz - rb->buff_size;
        }
        else
        {
            rb->head = (char*)rb->head + delSz;
        }
    }

    return UL_SUCCESS;
}

/**
 * @function: 
 * @description: 删除count个元素
 * @param {ul_ringbuffer_t} *rb
 * @param {uint8_t} count
 * @return {*}
 */
int ul_ringbuffer_del_elements( ul_ringbuffer_t *rb, uint8_t count )
{
    int ret,delSz;
    
    if( rb->head == rb->tail )
    {
        ret = UL_INVAILD;
    }
    else
    {
        delSz = count*rb->element_size;
        
        if( (char*)rb->head + delSz >= rb->buff_size + (char*)rb->buff )
        {
            rb->head = (char*)rb->head + delSz - rb->buff_size;
        }
        else
        {
            rb->head = (char*)rb->head + delSz;
        }
    }

    return UL_SUCCESS;
}

/**
 * @description: 添加一个元素
 * @param {ul_ringbuffer_t} *rb
 * @param {void*} elem
 * @return {*}
 */
int32_t ul_ringbuffer_add_element( ul_ringbuffer_t *rb, void* elem )
{
    int num;

    if( elem == NULL || rb == NULL )
        return UL_INVAILD;
    
    //计算剩余数量
    num = ul_ringbuffer_get_free_element_number( rb );
    if( num == 0 ){
        ul_ringbuffer_del_element( rb );
    }

    memcpy( rb->tail, elem, rb->element_size );
    (char*)rb->tail += rb->element_size;

    if( rb->tail == rb->buff_size + (char*)rb->buff  ){
        rb->tail = rb->buff;
    }
    
    return num;
}

/**
 * @description: 添加count个元素
 * @param {ul_ringbuffer_t} *rb
 * @param {void*} elems
 * @param {uint8_t} count
 * @return {*}
 */
int32_t ul_ringbuffer_add_elements( ul_ringbuffer_t *rb, void* elems, uint8_t count )
{
    if( elems == NULL || rb == NULL )
        return UL_INVAILD;

    for( int i = 0; i < count; i++ ){
        ul_ringbuffer_add_element( rb, elems );
        (char*)elems += rb->element_size;
    }

    return count;	
}

/**
 * @description: 获取一个元素
 * @param {ul_ringbuffer_t} *rb
 * @param {void*} elem
 * @return {*}
 */
int32_t ul_ringbuffer_get_element( ul_ringbuffer_t *rb, void* elem )
{
    int num;

    if( elem == NULL || rb == NULL )
        return UL_INVAILD;
    
    //计算剩余数量
    num = ul_ringbuffer_get_free_element_number( rb );
    if( num == 0 ){
        return UL_NOVALUE;
    }

    memcpy( elem, rb->head, rb->element_size );
    
    return UL_SUCCESS;
}

/**
 * @description: 获取count个元素
 * @param {ul_ringbuffer_t} *rb
 * @param {void*} elems
 * @param {uint8_t} count
 * @return {*}
 */
int32_t ul_ringbuffer_get_elements( ul_ringbuffer_t *rb, void* elems, uint8_t count )
{
    int32_t ret,i;

    for( i=0; i<count; i++ )
    {
        ret = ul_ringbuffer_get_element( rb, elems );
        if( ret!= UL_SUCCESS )
        {
            break;
        }
        (char*)elems += rb->element_size;
    }

    return i;
}

