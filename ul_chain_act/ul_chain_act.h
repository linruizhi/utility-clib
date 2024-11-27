/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 10:33:48
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-22 14:53:54
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_CHAIN_ACT_H__
#define __UL_CHAIN_ACT_H__

#include "ul_chain.h"
#include "libarch.h"


typedef void ( *ul_act_callback )( ul_chain_t** head, uint8_t arg_id, int ret );//bleh_err_em

typedef enum
{
    UL_ACT_FLAG_PERIODIC = 0,    //重复发送标志
    UL_ACT_FLAG_WAITACK =1,      //等待响应标志
    UL_ACT_FLAG_MUSTOK =2,       //必须通信成功标志
} ul_act_flags;


typedef enum
{
    UL_ACT_QUEUING,     /* 入队等待中*/
    UL_ACT_DELAYED,     /* 延迟到下一个周期，注意不可能延迟多次*/
    UL_ACT_WAITING_ACK, /* 等待响应 */
    UL_ACT_FINISHED,    /* 指令正确响应,出队列 */ 
    UL_ACT_FAILED,      /* 指令错误响应,出队列 */ 
    UL_ACT_TIMEOUT_TOO_MANY,    /* 指令完成,出队列 */
    UL_ACT_HOLDING, /* 用来阻塞指令队列，不会发出任务数据，但依旧会占用超时时间*/
    UL_ACT_PAUSING, /* 暂停该指令；通常用来执行特殊事件时，停止掉普通的指令*/
    UL_ACT_RESEND,  /* 重传状态 */
//    UL_ACT_PAUSING_ONCE, /* 暂停一次，*/
} ul_act_status;

typedef struct
{
    uint8_t act;
    uint8_t* data;
    uint8_t len;
    uint8_t flags;
    uint32_t timeout_ms;
} ul_act_attr_t;

typedef struct
{
    uint8_t* data;  //act数据区,用于数据组包
    uint8_t len;    //数据区长度时
    uint8_t arg;

    uint8_t act;        //根据实际情况
    uint8_t flags;      //ul_act_flags 按位与
    ul_act_status sta;  //act当前状态

    uint8_t delayed_cnt;  //超时计数
    uint8_t delayed_max;  //超时容忍度,相当于重传次数
    
    uint32_t timeout_ms;  //超时时间
    uint32_t trig_time;   //触发时间,当前时间-触发时间>超时时间,即为超时
    
    ul_act_callback callback;
} ul_act_t;

int ul_chain_act_poll( ul_chain_t** head, ul_chain_t** end );
ul_chain_t* ul_chain_act_create( ul_act_attr_t attr, ul_act_callback fn );
int ul_chain_act_release( ul_chain_t* node );
int ul_chain_act_add( ul_chain_t** head, ul_chain_t** tail,ul_chain_t* node );
int ul_chain_act_del( ul_chain_t** head, ul_chain_t** tail, ul_chain_t* node );
int ul_chain_act_set_status( ul_chain_t* head, uint8_t act ,ul_act_status sta  );
ul_act_t* ul_act_list_get_valid_act( ul_chain_t* head );


#endif // !__UL_CHAIN_ACT_H__