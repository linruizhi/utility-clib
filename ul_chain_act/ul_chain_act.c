/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 10:33:30
 * @LastEditors: linruizhi1124@outlook.com linruizhi1124@outlook.com
 * @LastEditTime: 2024-11-28 17:46:53
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */

#include "ul_chain_act.h"

/**
 * @description: 创建act chain 节点
 * @param {ul_act_attr_t} attr
 * @param {ul_act_callback} fn
 * @return {*}
 */
ul_chain_t* ul_chain_act_create( ul_act_attr_t attr, ul_act_callback fn )
{
    ul_chain_t* node;

    node = ul_chain_create();
    if( node != NULL )
    {
        //构建act
        node->detail = libc_malloc( sizeof( ul_act_t ));
        if( node->detail != NULL )
        {
            (( ul_act_t* )node->detail)->act = attr.act;
            (( ul_act_t* )node->detail)->data = attr.data;
            (( ul_act_t* )node->detail)->len = attr.len;
            (( ul_act_t* )node->detail)->flags = attr.flags;
            (( ul_act_t* )node->detail)->timeout_ms = attr.timeout_ms;
            (( ul_act_t* )node->detail)->callback = fn;
        }else{
            //构建act-失败
            ul_chain_release( node );
            node = NULL;
        }
    }

    return node;
}

/**
 * @function: 
 * @description: 释放act节点
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_act_release( ul_chain_t* node )
{
    ul_act_t* detail;

    if( node != NULL)
    {
        if( node->detail != NULL )
        {
            detail = node->detail;
            if( detail->data != NULL )
            {
                //释放act内容
                libc_free( detail->data );
                detail->data = NULL;
            }
            //释放act结构
            libc_free( detail );
            node->detail = NULL;
        }
        //释放节点
        libc_free( node );
    }

    return UL_SUCCESS;
}

/**
 * @function: 
 * @description: 将节点插入链表
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} tail
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_act_add( ul_chain_t** head, ul_chain_t** tail,ul_chain_t* node ){
    ul_chain_add( head, tail, node );
}

/**
 * @function: 
 * @description: 从链表中删除节点
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} tail
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_act_del( ul_chain_t** head, ul_chain_t** tail, ul_chain_t* node ){
    ul_chain_del( head, tail, node );
}

/**
 * @function: 
 * @description: 设置act状态
 * @param {ul_chain_t*} head
 * @param {uint8_t} act
 * @return {*}
 */
int ul_chain_act_set_status( ul_chain_t* head, uint8_t act ,ul_act_status sta  )
{
    int ret = UL_SUCCESS;
    ul_chain_t* tmp;
    ul_act_t* detail;

    for( tmp = head; tmp != NULL; tmp = tmp->next )
    {
        detail = tmp->detail;
        if( detail != NULL )
        {
            if( detail->act == act )
            {
                detail->sta = sta;
                detail->delayed_cnt = 0;
            }
            break;
        }
    }

    return ret;
}

/**
 * @function: 
 * @description: 获取可发送的act结构
 * @param {ul_chain_t*} head
 * @return {*}
 */
ul_act_t* ul_act_list_get_valid_act( ul_chain_t* head )
{
    ul_chain_t* tmp;
    ul_act_t* detail = NULL;
    
    for ( tmp = head; tmp != NULL; tmp = tmp->next ) 
    {
        detail = head->detail;  //获取act

        //存在act处于 等待响应状态 或者 状态，跳出
        if ( detail->sta == UL_ACT_WAITING_ACK || detail->sta == UL_ACT_HOLDING ) {
            tmp = NULL;
            break;
        }

        //act处于 可发送状态
        if ( detail->sta == UL_ACT_QUEUING )
        {
            //判断该act的标志，变更执行后状态
            if( detail->flags & ( 1 << UL_ACT_FLAG_WAITACK ) )
            {
                //需要等待响应
                detail->sta = UL_ACT_WAITING_ACK;
            }
            else
            {
                //不需要等待响应
                detail->sta = UL_ACT_FINISHED;
            }
            break;
        }
    }

    return detail;
}

/**
 * @function: 
 * @description: act状态回调函数
 * @param {ul_chain_t**} head
 * @param {ul_act_t*} act
 * @return {*}
 */
int ul_chain_act_callback( ul_chain_t** head, ul_act_t act )
{
    if ( act.callback != NULL ) {
        if ( act.sta == UL_ACT_FINISHED ) {
            act.callback( head, act.arg, UL_SUCCESS ); //执行成功
        } else if ( act.sta == UL_ACT_TIMEOUT_TOO_MANY ) {
            act.callback( head, act.arg, UL_TIMEOUT ); //超时
        } else if ( act.sta == UL_ACT_FAILED ) {
            act.callback( head, act.arg, UL_INVAILD ); //执行错误
        }
    }
}

/**
 * @function: 
 * @description: 尝试移除指定节点，并进行节点释放
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} end
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_act_try_remove( ul_chain_t** head, ul_chain_t** end, ul_chain_t* node )
{
    ul_act_t* act;
    int ret = UL_NOVALUE;

    if( node && node->detail )
    {
        act = node->detail; //获取act节点

        //检查act是否需要重发
        if( !( act->flags & (1 << UL_ACT_FLAG_PERIODIC)) )
        {
            //不需要重发，判断当前act状态
            //act结束状态（finish、timeout、failed），进行act释放
            if( act->sta == UL_ACT_FINISHED || act->sta == UL_ACT_TIMEOUT_TOO_MANY || act->sta == UL_ACT_FAILED ){
                ul_chain_act_del( head, end, node);
                ul_chain_act_release( node );
                ret = UL_SUCCESS;
            }
        }
    }
    
    return ret;
}


 /**
  * @function: 
  * @description: 循环执行act，应用接口
  * @param {bleh_act_t*} root
  * @return {*}
  */
int ul_chain_act_poll( ul_chain_t** head, ul_chain_t** end, uint32_t time_ms )
{
    int ret;
    ul_chain_t* tmp;
    ul_act_t* detail;

    for ( tmp = *head; tmp != NULL; tmp = tmp->next )
    {
        detail = tmp->detail; //获取act

        if( detail == NULL )
        {
            //act节点为空，跳过(异常)
            continue;
        }
        
        if( detail->sta == UL_ACT_WAITING_ACK )//等待响应
        {
            //act等待超时，trig_time（触发时间），timeout_ms（等待阈值），当前时间
            if( detail->trig_time + detail->timeout_ms > time_ms )
            {
                detail->delayed_cnt ++; //超时计数
                if( detail->delayed_max <= detail->delayed_cnt )    //判断超时容忍
                {
                    //标记act超时
                    detail->sta = UL_ACT_TIMEOUT_TOO_MANY;
                }
                else
                {
                    //在超时容忍范围内，重传
                    detail->sta = UL_ACT_QUEUING;
                }
            }
        }

        //act回调
        ul_chain_act_callback( head, *detail );

        //尝试清除act
        if( ul_chain_act_try_remove( head, end, tmp) == UL_SUCCESS )
        {
            //清除成功后,tmp地址的内容被执行为NULL
            //这里是跳出操作
            //可以更换为手动更新tmp
            break;
        }
    }

    return ret;
}
