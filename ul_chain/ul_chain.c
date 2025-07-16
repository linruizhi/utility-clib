/*
 * @Author: linruizhi
 * @Description:
 * @Date: 2024-11-22 10:12:59
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-12-10 18:11:21
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved.
 */
#include "ul_chain.h"

/**
 * @function:
 * @description:
 * @return {*}
 */
ul_chain_lock_t *ul_chain_lock_create(void)
{
    ul_chain_lock_t *lock;

    lock = (ul_chain_lock_t *)libc_malloc(sizeof(ul_chain_lock_t));
    if (lock)
    {
        *lock = libcOsMutexNew();
    }

    return lock;
}



/**
 * @function:
 * @description:
 * @return {*}
 */
ul_chain_t *ul_chain_create(void)
{
    ul_chain_t *tmp;

    tmp = libc_malloc(sizeof(ul_chain_t));
    if (tmp != NULL) {
        tmp->next   = NULL;
        tmp->prev   = NULL;
        tmp->detail = NULL;
    }

    return tmp;
}

/**
 * @brief Releases the memory allocated for a chain node.
 *
 * This function frees the memory associated with the given chain node.
 *
 * @param node Pointer to the ul_chain_t node to be released.
 *
 * @return void This function does not return a value.
 */
void ul_chain_release(ul_chain_t *node)
{
    if (node != NULL) {
        if (node->detail != NULL) {
            libc_free(node->detail);
            node->detail = NULL;
        }
        libc_free(node);
    }
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

    return UL_ERR_SUCCESS;
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} tail
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_add(ul_chain_t **head, ul_chain_t **tail, ul_chain_t *node)
{
    node->next = NULL;

    if (*head == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    node->prev = (*tail);
    *tail      = node;

    return UL_ERR_SUCCESS;
}

int ul_chain_insert_node_to_head(ul_chain_t **head, ul_chain_t **tail, ul_chain_t *node)
{
    if( node == NULL ) {
        return UL_ERR_STATUS_ERROR;
    }

    if( *head == NULL ) {
        *head = *tail = node;
        node->prev = node->next = NULL;
        return UL_ERR_SUCCESS;
    }

    node->next = *head;
    node->prev = NULL;
    (*head)->prev = node;
    *head = node;

    return UL_ERR_SUCCESS;
}

int ul_chain_insert_node_to_tail(ul_chain_t **head, ul_chain_t **tail, ul_chain_t *node)
{
    if( node == NULL ) {
        return UL_ERR_STATUS_ERROR;
    }

    if( *head == NULL ) {
        *head = *tail = node;
        node->prev = node->next = NULL;
        return UL_ERR_SUCCESS;
    }

    node->prev = *tail;
    node->next = NULL;
    (*tail)->next = node;
    *tail = node;

    return UL_ERR_SUCCESS;
}

int ul_chain_insert_node_to_position(ul_chain_t **head, ul_chain_t **tail, ul_chain_t *node, ul_chain_position_t pos)
{
    if( node == NULL ) {
        return UL_ERR_STATUS_ERROR;
    }

    if (*head == NULL ) {
        *head = *tail = node;
        node->prev = node->next = NULL;
        return UL_ERR_SUCCESS;
    }

    switch (pos) {
        case UL_CHAIN_POSITION_HEAD:
            ul_chain_insert_node_to_head(head, tail, node);
            break;
        case UL_CHAIN_POSITION_TAIL:
            ul_chain_insert_node_to_tail(head, tail, node);
            break;
        default:
            return UL_ERR_STATUS_ERROR;
    }
    return UL_ERR_SUCCESS;
}



int ul_chain_del_node_form_specified(ul_chain_t **head, ul_chain_t **tail, ul_chain_t *node)
{
    if( *head == NULL || *tail == NULL || node == NULL ) {
        return UL_ERR_STATUS_ERROR;
    }

    if (*head == *tail && *head == node) {
        *head = *tail = NULL;
        node->prev = node->next = NULL;
        return UL_ERR_SUCCESS;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node->next = NULL;
    return UL_ERR_SUCCESS;
}

int ul_chain_del_node_form_head(ul_chain_t **head, ul_chain_t **tail, ul_chain_t **node)
{
    if( *head == NULL || *tail == NULL ) {
        *node = NULL;
        return UL_ERR_STATUS_ERROR;
    }

    *node = *head;
    *head = (*node)->next;
    if (*head) {
        (*head)->prev = NULL;
    } else {
        *tail = NULL;
    }

    (*node)->prev = (*node)->next = NULL;
    return UL_ERR_SUCCESS;
}

int ul_chain_del_node_form_tail(ul_chain_t **head, ul_chain_t **tail, ul_chain_t **node)
{
    if( *head == NULL || *tail == NULL ) {
        (*node) = NULL;
        return UL_ERR_STATUS_ERROR;
    }

    (*node) = *tail;
    *tail = (*node)->prev;
    if (*tail) {
        (*tail)->next = NULL;
    } else {
        *head = NULL;
    }

    (*node)->prev = (*node)->next = NULL;
    return UL_ERR_SUCCESS;
}

int ul_chain_del_node_form_position(ul_chain_t **head, ul_chain_t **tail, ul_chain_t **node, ul_chain_position_t pos )
{
    if( *head == NULL || *tail == NULL ) {
        *node = NULL;
        return UL_ERR_STATUS_ERROR;
    }

    if( pos == UL_CHAIN_POSITION_HEAD ) {
        return ul_chain_del_node_form_head(head, tail, node);
    } else if( pos == UL_CHAIN_POSITION_TAIL ) {
        return ul_chain_del_node_form_tail(head, tail, node);
    } else {
        return ul_chain_del_node_form_specified(head, tail, *node);
    }
}
