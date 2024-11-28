/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 17:17:40
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-22 17:34:17
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_RINGBUFFER_H__
#define __UL_RINGBUFFER_H__

#include "libarch.h"

typedef struct {
    uint16_t element_size;  //元素大小
    uint16_t capacity;  //元素容积量
    uint32_t buff_size; //内存区长度，为element_size*capacity
    void  *head; //头指针（读指针）
    void  *tail; //尾指针（写指针）
    void  *buff; //内存区
}ul_ringbuffer_t;


typedef struct {
    uint16_t element_size;
    uint16_t capacity;
    void  *buff; //内存区,如果内存区为NULL,则会动态申请
}ul_rb_attr_t;

ul_ringbuffer_t* ul_ringbuffer_create( ul_rb_attr_t attr  );
int ul_ringbuffer_init( ul_ringbuffer_t *rb, ul_rb_attr_t attr );
int ul_ringbuffer_get_element_number( ul_ringbuffer_t *rb );
int ul_ringbuffer_get_free_element_number( ul_ringbuffer_t *rb );
int ul_ringbuffer_del_element( ul_ringbuffer_t *rb );
int ul_ringbuffer_del_elements( ul_ringbuffer_t *rb, uint8_t count );
int32_t ul_ringbuffer_add_element( ul_ringbuffer_t *rb, void* elem );
int32_t ul_ringbuffer_add_elements( ul_ringbuffer_t *rb, void* elems, uint8_t count );
int32_t ul_ringbuffer_get_element( ul_ringbuffer_t *rb, void* elem );
int32_t ul_ringbuffer_get_elements( ul_ringbuffer_t *rb, void* elems, uint8_t count );
#endif // !__UL_RINGBUFFER_H__

