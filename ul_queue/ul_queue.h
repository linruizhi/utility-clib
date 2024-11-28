/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 17:17:40
 * @LastEditors: linruizhi1124@outlook.com linruizhi1124@outlook.com
 * @LastEditTime: 2024-11-28 16:09:56
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_RINGBUFFER_H__
#define __UL_RINGBUFFER_H__

#include "libarch.h"

typedef struct {
    uint16_t element_number;//元素数量
    uint16_t capacity;      //元素容积量
    void  **head;   //头指针（读指针）
    void  **tail;   //尾指针（写指针）
    void  **queue;  //队列地址，相当于数组指针，每个地址指向一个数组
}ul_queue_t;

ul_queue_t* ul_queue_create( uint16_t  capacity );
int ul_queue_get_element_number( ul_queue_t *queue );
int ul_queue_get_free_element_number( ul_queue_t *queue );
int ul_queue_is_empty( ul_queue_t *queue );
int ul_queue_is_full( ul_queue_t *queue );
int ul_queue_del_element( ul_queue_t *queue );
int32_t ul_queue_add_element( ul_queue_t *queue, void* elem );
int32_t ul_queue_get_element( ul_queue_t *queue, void* elem );
int32_t ul_queue_search_index_element( ul_queue_t *queue, uint16_t index, void** elem );
int32_t ul_queue_del_index_element( ul_queue_t *queue, uint16_t index );
#endif // !__UL_RINGBUFFER_H__

