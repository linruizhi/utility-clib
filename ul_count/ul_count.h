/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-08-27 15:16:34
 * @LastEditors: linruizhi
 * @LastEditTime: 2025-06-09 15:02:09
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_COUNT_H__
#define __UL_COUNT_H__
#include <stdint.h>
#include <time.h>

#define COUNT_INFO_LIST_MUTEX libcOsMutex_t
#define COUNT_INFO_LIST_MUTEX_NEW libcOsMutexNew
#define COUNT_INFO_LIST_MUTEX_LOCK libcOsMutexLock
#define COUNT_INFO_LIST_MUTEX_UNLOCK libcOsMutexUnlock

#warning 需要修改定义
#define UL_COUNT_TIME_T uint32_t

typedef struct count_info{
    uint32_t tframe_count;      /* 发送帧计数 */
    uint32_t rframe_count;      /* 接收帧计数 */
    uint32_t tbyte_count;       /* 发送字节数量,单位为Byte */
    uint32_t rbyte_count;       /* 接收字节数量,单位为Byte */
    uint32_t tbps;              /* 每秒字节量 */
    uint32_t rbps;              /* 每秒字节量 */
    uint32_t tfps;              /* 每秒帧数 */
    uint32_t rfps;              /* 每秒帧数 */
    UL_COUNT_TIME_T last_time;         /* 最后活跃时间,系统运行时间,单位:秒 */
}ul_count_info_t;

typedef struct count_info_list{
    uint16_t id;            /* 节点ID,用于区分节点. 节点ID与被统计者之间的映射关系,请自行维护 */
    uint32_t tframe_count;  /* 发送帧计数 */
    uint32_t rframe_count;  /* 接收帧计数 */
    uint32_t tbyte_count;   /* 发送字节数量 */
    uint32_t rbyte_count;   /* 接收字节数量 */
    uint32_t tbps;  /* 每秒字节量 */
    uint32_t tfps;  /* 每秒帧数 */
    uint32_t rbps;  /* 每秒字节量 */
    uint32_t rfps;  /* 每秒帧数 */
    UL_COUNT_TIME_T last_time; /* 最后活跃时间,系统运行时间 */

    struct count_info_list *next;
}count_info_list_t;

//外设通信统计
typedef struct count_peripheral_info{
    uint32_t tbyte_count;       /* 发送字节数量,单位为Byte */
    uint32_t rbyte_count;       /* 接收字节数量,单位为Byte */
    uint32_t tbps;              /* 每秒字节量 */
    uint32_t rbps;              /* 每秒字节量 */
    UL_COUNT_TIME_T last_rx_time;      /* 最后活跃时间,系统运行时间,单位:秒 */
    UL_COUNT_TIME_T last_tx_time;      /* 最后活跃时间,系统运行时间,单位:秒 */
}count_peripheral_info_t;

count_info_list_t* ul_create_countList( void );

#endif// #define __UL_COUNT_H__