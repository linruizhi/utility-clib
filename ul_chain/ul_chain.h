/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-21 17:55:12
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-22 14:54:32
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_CHAIN_H__
#define __UL_CHAIN_H__


typedef struct _UL_CHAIN__
{   
    void* detail; //链表内容
    
    struct _UL_CHAIN__* next;

    struct _UL_CHAIN__* prev;
} ul_chain_t;

ul_chain_t* ul_chain_create( void );
void ul_chain_release( ul_chain_t* node );
int ul_chain_add( ul_chain_t** head, ul_chain_t** tail,ul_chain_t* node );
int ul_chain_del( ul_chain_t** head, ul_chain_t** tail, ul_chain_t* node );

#endif // !__UL_CHAIN_H__