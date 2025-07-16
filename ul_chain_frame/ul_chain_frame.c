/*
 * @file: ul_chain_frame.c
 * @brief: Implementation of the chain frame utility functions
 * 
 * This file contains the implementation of functions declared in ul_chain_frame.h.
 * It provides functionality for creating, manipulating, and managing frame chains,
 * which are essentially linked lists of data frames.
 * 
 * Key features implemented:
 * - Creation of frame chains with specified length or from existing data
 * - Memory management for frame chains and their associated data buffers
 * - Insertion and deletion of nodes at different positions in the chain
 * - Thread-safe operations using mutex locks
 * - Empty state checking for frame chains
 * 
 * The implementation uses a combination of chain operations and data buffer
 * management to provide a flexible and efficient frame handling system.
 * Thread safety is ensured through the use of mutex locks in relevant operations.
 * 
 * @author: linruizhi
 * @date: 2024-11-22
 * @last_modified: 2025-01-03
 * 
 * @copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved.
 */
#include "ul_chain_frame.h"
#include "ul_data_buffer/ul_data_buffer.h"
#include "ul_error.h"
/**
 * @function:
 * @description:
 * @return {*}
 */
ul_frame_chain_node_t *ul_chain_frame_create_with_len( uint16_t frame_len )
{
    ul_frame_chain_node_t *node;
    ul_data_buffer_t * data_buffer;

    // Allocate memory for the node
    node = (ul_frame_chain_node_t*)ul_chain_create();
    if (node != NULL)
    {
        data_buffer = ul_data_buffer_create_with_size(frame_len);
        if (data_buffer == NULL)
        {
            ul_chain_frame_free(node);
            node = NULL;
        }
        else
        {
            node->frame = data_buffer;
        }
    }

    return node;
}

ul_frame_chain_node_t *ul_chain_frame_create_with_data( uint8_t *data, uint16_t len )
{
    ul_frame_chain_node_t *node;
    ul_data_buffer_t * data_buffer;

    // Allocate memory for the node
    node = (ul_frame_chain_node_t*)ul_chain_create();
    if (node != NULL)
    {
        data_buffer = ul_data_buffer_create_with_data(data, len);
        if (data_buffer == NULL)
        {
            ul_chain_frame_free(node);
            node = NULL;
        }
        else
        {
            node->frame = data_buffer;
        }
    }

    return node;
}

int ul_chain_frame_free(ul_frame_chain_node_t *node)
{
    if (node != NULL)
    {
        if (node->frame != NULL)
        {
            ul_data_buffer_destroy(node->frame);
            node->frame = NULL;
        }

        libc_free(node);
        node = NULL;
    }

    return UL_ERR_SUCCESS;
}


int ul_chain_frame_insert_node_to_posotion(ul_frame_chain_t *chain, ul_frame_chain_node_t *node, ul_chain_position_t pos)
{
    if( chain == NULL )
    {
        return UL_ERR_INVAILD;
    }

    return ul_chain_insert_node_to_position((ul_chain_t**)&(chain->head), (ul_chain_t**)&(chain->tail), (ul_chain_t*)node, pos);
}

int ul_chain_frame_insert_node_to_posotion_with_lock(ul_frame_chain_t *chain, ul_frame_chain_node_t *node, ul_chain_position_t pos)
{
    if( chain == NULL )
    {
        return UL_ERR_INVAILD;
    }

    if( chain->lock == NULL )
    {
        return UL_ERR_INVAILD;
    }

    // Lock the chain
    libcOsMutexLock(*chain->lock);

    // Insert the node to the chain
    int ret = ul_chain_frame_insert_node_to_posotion(chain, node, pos);

    // Unlock the chain
    libcOsMutexUnlock(*chain->lock);

    return ret;
}


int ul_chain_frame_del_node_form_posotion(ul_frame_chain_t *chain, ul_frame_chain_node_t **node, ul_chain_position_t pos)
{
    if( chain == NULL )
    {
        return UL_ERR_INVAILD;
    }

    return ul_chain_del_node_form_position((ul_chain_t**)&(chain->head),(ul_chain_t**)&(chain->tail), (ul_chain_t**)node, pos);
}


int ul_chain_frame_del_node_form_posotion_with_lock(ul_frame_chain_t *chain, ul_frame_chain_node_t **node,ul_chain_position_t pos)
{
    if( chain == NULL )
    {
        return UL_ERR_INVAILD;
    }

    if( chain->lock == NULL )
    {
        return UL_ERR_INVAILD;
    }

    // Lock the chain
    libcOsMutexLock(*chain->lock);

    // Delete the node from the chain
    int ret = ul_chain_frame_del_node_form_posotion(chain, node, pos);

    // Unlock the chain
    libcOsMutexUnlock(*chain->lock);

    return ret;
}

int ul_chain_frame_is_empty(ul_frame_chain_t *chain)
{
    if( chain == NULL )
    {
        return UL_ERR_INVAILD;
    }

    return (chain->head == NULL)? 1 : 0;
}


int ul_chain_frame_is_empty_with_lock(ul_frame_chain_t *chain)
{
    if( chain == NULL )
    {
        return UL_ERR_INVAILD;
    }

    if( chain->lock == NULL )
    {
        return UL_ERR_INVAILD;
    }

    // Lock the chain
    libcOsMutexLock(*chain->lock);

    // Check if the chain is empty
    int ret = ul_chain_frame_is_empty(chain);

    // Unlock the chain
    libcOsMutexUnlock(*chain->lock);

    return ret;
}