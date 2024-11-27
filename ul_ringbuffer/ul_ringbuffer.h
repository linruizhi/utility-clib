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
    uint16_t element_size;
    uint16_t capacity;
    void  *head; 
    void  *tail;
    void  *buff; //内存区,内存区长度为element_size*capacity
}ul_ringbuffer_t;


typedef struct {
    uint16_t element_size;
    uint16_t capacity;
    void  *buff; //内存区,如果内存区为NULL,则会动态申请
}ul_rb_attr_t;




#endif // !__UL_RINGBUFFER_H__

