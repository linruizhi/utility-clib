/*
  * @file: ul_chain_frame.h
  * @brief: Header file for the chain frame utility
  * 
  * This file defines the structures and function prototypes for a chain frame
  * implementation. It provides a flexible framework for managing linked lists
  * of frame data, with support for thread-safe operations through locking
  * mechanisms.
  * 
  * The chain frame utility allows for creation, manipulation, and management
  * of frame chains, which can be used for various purposes such as buffering,
  * queuing, or processing of data frames in a linked structure.
  * 
  * Key features:
  * - Creation of frame chains with specified length or from existing data
  * - Insertion and deletion of nodes at different positions in the chain
  * - Thread-safe operations with customizable locking mechanisms
  * - Empty state checking
  * 
  * @author: linruizhi
  * @date: 2024-11-22
  * @last_modified: 2024-12-19
  * 
  * @copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved.
  */
#ifndef __UL_CHAIN_FRAME_H__
#define __UL_CHAIN_FRAME_H__

#include "ul_chain/ul_chain.h"
#include "ul_data_buffer/ul_data_buffer.h"
#include "libarch.h"

#ifndef FRAME_CHAIN_READ_LOCK
#define FRAME_CHAIN_READ_LOCK   libcOsMutexLock
#endif
#ifndef FRAME_CHAIN_WRITE_LOCK
#define FRAME_CHAIN_WRITE_LOCK  libcOsMutexLock
#endif
#ifndef FRAME_CHAIN_UNLOCK
#define FRAME_CHAIN_UNLOCK      libcOsMutexUnlock
#endif

typedef ul_chain_lock_t ul_frame_chain_lock_t;


typedef struct
{   
    ul_data_buffer_t* frame; //链表内容
    
    struct _UL_CHAIN__* next;

    struct _UL_CHAIN__* prev;
} ul_frame_chain_node_t;

typedef struct
{
    ul_frame_chain_node_t *head;
    ul_frame_chain_node_t *tail;
    ul_frame_chain_lock_t *lock;
} ul_frame_chain_t;

ul_frame_chain_node_t *ul_chain_frame_create_with_len( uint16_t frame_len );
ul_frame_chain_node_t *ul_chain_frame_create_with_data( uint8_t *data, uint16_t len );
int ul_chain_frame_free(ul_frame_chain_node_t *node);
int ul_chain_frame_insert_node_to_posotion(ul_frame_chain_t *chain, ul_frame_chain_node_t *node, ul_chain_position_t pos);
int ul_chain_frame_insert_node_to_posotion_with_lock(ul_frame_chain_t *chain, ul_frame_chain_node_t *node, ul_chain_position_t pos);
int ul_chain_frame_del_node_form_posotion(ul_frame_chain_t *chain, ul_frame_chain_node_t **node, ul_chain_position_t pos);
int ul_chain_frame_del_node_form_posotion_with_lock(ul_frame_chain_t *chain, ul_frame_chain_node_t **node, ul_chain_position_t pos);
int ul_chain_frame_is_empty(ul_frame_chain_t *chain);
int ul_chain_frame_is_empty_with_lock(ul_frame_chain_t *chain);

#endif // !__UL_CHAIN_FRAME_H__
