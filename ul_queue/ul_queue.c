/*
 * @Author: linruizhi
 * @Description: 元素大小固定，会覆盖原始数据，元素格式不定
 * @Date: 2024-11-22 17:17:33
 * @LastEditors: linruizhi1124@outlook.com linruizhi1124@outlook.com
 * @LastEditTime: 2024-11-28 16:40:35
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#include <stdio.h>
#include <stdint.h>
#include "ul_error.h"
#include "ul_queue.h"


/**
 * @description: 创建queue
 * @param {ul_queue_attr_t} attr
 * @return {*}
 */
ul_queue_t* ul_queue_create( uint16_t  capacity )
{
    ul_queue_t* queue = NULL;
    uint32_t size;

    if( capacity )
    {
        queue = libc_malloc( sizeof( ul_queue_t ) );
        if( queue )
        {
            queue->queue = libc_malloc( sizeof(void *)*capacity );
            if( queue->queue == NULL )
            {
                libc_free( queue );
                queue = NULL;
            }
        }

        if( queue )
        {
            queue->capacity = capacity;
            queue->element_number = 0;
            queue->head = queue->tail = queue->queue;
        }
    }

    return queue;
}

/**
 * @function: 
 * @description: 获取已存在的元素数量
 * @param {ul_queue_t} *queue
 * @return {*}
 */
int ul_queue_get_element_number( ul_queue_t *queue )
{
    int num;

    if( !(queue && queue->queue) ){
        return UL_INVAILD;
    }

    return queue->element_number;
}

/**
 * @description: 获取可写入元素的数量
 * @param {ul_queue_t} *queue
 * @return {*}
 */
int ul_queue_get_free_element_number( ul_queue_t *queue )
{
    int num;
    
    num = queue->capacity - ul_queue_get_element_number( queue );

    return num;
}

/**
 * @description: 
 * @param {ul_queue_t} *queue
 * @return {*}
 */
int ul_queue_is_empty( ul_queue_t *queue )
{
    if( !(queue && queue->queue) ){
        return UL_INVAILD;
    }

    return queue->element_number == 0 ? 1 : 0;
}

/**
 * @description: 
 * @param {ul_queue_t} *queue
 * @return {*}
 */
int ul_queue_is_full( ul_queue_t *queue )
{
    if( !(queue && queue->queue) ){
        return UL_INVAILD;
    }

    return queue->element_number >= queue->capacity ? 1 : 0;
}

/**
 * @description: 删除一个元素
 * @param {ul_queue_t} *queue
 * @return {*}
 */
int ul_queue_del_element( ul_queue_t *queue )
{
    int ret,delSz;

    if( !(queue && queue->queue) ){
        return UL_INVAILD;
    }

    if( !(queue->element_number ) ){
        return UL_NOVALUE;
    }
    
    if (++(queue->head) >= queue->queue + queue->capacity )
        queue->head = queue->queue;
    --(queue->element_number);

    return UL_SUCCESS;
}

/**
 * @description: 添加一个元素
 * @param {ul_queue_t} *queue
 * @param {void*} elem
 * @return {*}
 */
int32_t ul_queue_add_element( ul_queue_t *queue, void* elem )
{
    int num;

    if( elem == NULL || queue == NULL || queue->queue )
        return UL_INVAILD;

    if( ul_queue_is_full( queue )){
        return UL_NOMEMARY;
    }
    
    *(queue->tail)++ = elem;
    if (queue->tail == queue->queue + queue->capacity)
        queue->tail = queue->queue;
    ++(queue->element_number);
    
    return num;
}

/**
 * @description: 获取一个元素
 * @param {ul_queue_t} *queue
 * @param {void*} elem
 * @return {*}
 */
int32_t ul_queue_get_element( ul_queue_t *queue, void** elem )
{
    int num;

    if( elem == NULL || queue == NULL || queue->queue )
        return UL_INVAILD;

    if( ul_queue_is_empty(queue) )
        return UL_NOMEMARY;
    
    *elem = *(queue->head);
    
    return UL_SUCCESS;
}

/**
 * @description: 
 * @param {ul_queue_t} *queue
 * @param {uint16_t} index
 * @param {void**} elem
 * @return {*}
 */
int32_t ul_queue_search_index_element( ul_queue_t *queue, uint16_t index, void** elem )
{
    void **ptr;

    if( elem == NULL || queue == NULL || queue->queue )
        return UL_INVAILD;

    if( index >= queue->element_number )
        return UL_INVAILD;

    ptr = queue->head + index;
    if ( ptr >= queue->queue + queue->capacity )
        ptr = queue->queue + (ptr - (queue->queue + queue->element_number));
    
    *elem = *ptr;

    return UL_SUCCESS;
}

/**
 * @description: 
 * @param {ul_queue_t} *queue
 * @param {uint16_t} index
 * @return {*}
 */
int32_t ul_queue_del_index_element( ul_queue_t *queue, uint16_t index )
{
    
}
