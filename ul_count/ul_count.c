/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2025-02-19 15:57:11
 * @LastEditors: linruizhi
 * @LastEditTime: 2025-05-30 16:44:11
 * @Copyright: Copyright (c) 2025 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#include "ul_count.h"
#include "string.h"
#include "libarch.h"

#define COUNT_MALLOC libc_malloc
#define COUNT_CALLOC libc_calloc

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 每个清理周期,应当保存统计,保存之后清理
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void count_clear( ul_count_info_t* count )
{
    /* 保存 */

    /* 清理 */
    memset( count, 0, sizeof( ul_count_info_t ) );
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void count_prints( ul_count_info_t count )
{
 
}

/**
 * @function: 
 * @description: 
 * @return {*}
 */
count_info_list_t* ul_create_countList( void )
{
    count_info_list_t* list;

    list = COUNT_CALLOC( sizeof( count_info_list_t ) );

    return list;
}


/**
 * @function: 
 * @description: 
 * @return {*}
 */
count_peripheral_info_t* ul_count_create_peripheral_info_node( void )
{
    return NULL;
}