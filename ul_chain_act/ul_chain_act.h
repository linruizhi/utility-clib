/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 10:33:48
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-12-19 17:50:55
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_CHAIN_ACT_H__
#define __UL_CHAIN_ACT_H__

#include "ul_chain/ul_chain.h"

#define ACT_CHAIN_READ_LOCK   libcOsMutexLock
#define ACT_CHAIN_WRITE_LOCK  libcOsMutexLock
#define ACT_CHAIN_UNLOCK      libcOsMutexUnlock

typedef enum
{
    UL_ACT_FLAG_PERIODIC = 0,   //重复发送标志
    UL_ACT_FLAG_WAITACK =1,     //等待响应标志
    UL_ACT_FLAG_MUSTOK,         //必须通信成功标志
} ul_act_flags;


typedef enum
{
    UL_ACT_QUEUING,     /* 入队等待中*/
    UL_ACT_DELAYED,     /* 等待到超时后再进入发送状态 */
    UL_ACT_WAITING_ACK, /* 等待响应 */
    // UL_ACT_FINISH_DELAY,/* 指令响应完成,不出队列*/
    UL_ACT_FINISHED,    /* 指令正确响应,出队列 */
    UL_ACT_FAILED,      /* 指令错误响应,出队列 */
    UL_ACT_TIMEOUT_TOO_MANY,    /* 指令完成,出队列 */
    UL_ACT_HOLDING,     /* 用来阻塞指令队列，不会发出任务数据，但依旧会占用超时时间*/
    UL_ACT_PAUSING,     /* 暂停该指令；通常用来执行特殊事件时，停止掉普通的指令*/
    UL_ACT_RESEND,      /* 重传状态 */
    UL_ACT_ABANDON,     /* 弃用该指令 */
} ul_act_status;

typedef struct
{   
    ul_chain_t *head;
    ul_chain_t *tail;
    ul_chain_lock_t *lock;
} ul_act_chain_t;

struct ul_act;

typedef void ( *ul_act_callback )( ul_act_chain_t *chain, struct ul_act* now_act );

typedef struct
{
    uint8_t act;
    uint8_t* data;
    uint8_t len;
    void* arg;
    uint8_t delayed_max;
    uint32_t flags;
    uint32_t timeout_ms;
    ul_act_callback callback;

} ul_act_attr_t;

typedef struct ul_act
{
    uint8_t* data;  //act数据区,用于数据组包
    uint8_t len;    //数据区长度时
    void* arg;   //act自变量

    uint8_t act;        //根据实际情况
    uint8_t flags;      //ul_act_flags 按位与
    ul_act_status sta;  //act当前状态

    uint8_t delayed_cnt;  //超时计数
    uint8_t delayed_max;  //超时容忍度,相当于重传次数
    
    uint32_t timeout_ms;  //超时时间,如果标记wait_ack,优先级高于PERIODIC,如果仅PERIODIC,则为发送间隔
    uint64_t trig_time;   //触发时间,当前时间-触发时间>超时时间,即为超时
    
    ul_act_callback callback;
} ul_act_t;


int ul_chain_act_poll( ul_act_chain_t* chain, uint64_t time );
ul_chain_t* ul_chain_act_create( ul_act_attr_t attr );
int ul_chain_act_release( ul_chain_t* node );
int ul_chain_act_add( ul_act_chain_t* chain,ul_chain_t* node );
int ul_chain_act_set_status( ul_chain_t* head, uint8_t act ,ul_act_status sta  );
ul_act_t* ul_act_list_get_valid_act( ul_act_chain_t chain, uint64_t time, int *ret );
int ul_chain_act_set_all_pause( ul_act_chain_t chain );
int ul_chain_act_set_all_resume( ul_act_chain_t chain );
ul_errcode_t ul_chain_act_get_arg( ul_chain_t* head, uint8_t act, void** arg );
ul_errcode_t ul_chain_act_check_existed( ul_chain_t* head, uint8_t act );

#endif // !__UL_CHAIN_ACT_H__
