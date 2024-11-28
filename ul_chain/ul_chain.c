/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 10:12:59
 * @LastEditors: linruizhi1124@outlook.com linruizhi1124@outlook.com
 * @LastEditTime: 2024-11-28 17:28:59
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#include "libarch.h"
#include "ul_chain.h"
/**
 * @function: 
 * @description: 创建节点
 * @return {*}
 */
ul_chain_t* ul_chain_create( void )
{
    ul_chain_t* tmp;
    
    tmp = libc_malloc( sizeof( ul_chain_t ));
    if ( tmp != NULL ) {

        tmp->next = NULL;
        tmp->prev = NULL;
        tmp->detail = NULL;
    }

    return tmp;
}

/**
 * @description: 释放节点
 * @param {ul_chain_t*} node
 * @return {*}
 */
void ul_chain_release( ul_chain_t* node )
{
    libc_free( node );
}

/**
 * @function: 
 * @description: 
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} tail
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_add( ul_chain_t** head, ul_chain_t** tail,ul_chain_t* node )
{
    node->next = NULL;

    if (*head == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    node->prev = (*tail);
    *tail = node;
}

/**
 * @function: 
 * @description: 
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} tail
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_del( ul_chain_t** head, ul_chain_t** tail, ul_chain_t* node )
{
    if (*head == node) {
        if (*tail == node) {
            *head = *tail = NULL;
        } else {
            *head = node->next;
            (*head)->prev = NULL;
        }
    } else {
        if (*tail == node) {
            *tail = node->prev;
            (*tail)->next = NULL;
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    }
    node->prev = node->next = NULL;
}

