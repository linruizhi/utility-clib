/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 10:33:30
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-22 14:45:47
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */

#include "ul_chain_act.h"

/**
 * @function: 
 * @description: 
 * @return {*}
 */
ul_chain_t* ul_chain_act_create( ul_act_attr_t attr, ul_act_callback fn )
{
    ul_chain_t* node;

    node = ul_chain_create();
    if( node != NULL )
    {
        node->detail = libc_malloc( sizeof( ul_act_t ));
        if( node->detail != NULL )
        {
            (( ul_act_t* )node->detail)->act = attr.act;
            (( ul_act_t* )node->detail)->data = attr.data;
            (( ul_act_t* )node->detail)->len = attr.len;
            (( ul_act_t* )node->detail)->flags = attr.flags;
            (( ul_act_t* )node->detail)->timeout_ms = attr.timeout_ms;
            (( ul_act_t* )node->detail)->callback = fn;
        }
        else{
            ul_chain_release( node );
            node = NULL;
        }
    }

    return node;
}

/**
 * @function: 
 * @description: 
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
                libc_free( detail->data );
                detail->data = NULL;
            }
            libc_free( detail );
            node->detail = NULL;
        }

        libc_free( node );
    }

    return SUCCESS;
}

/**
 * @function: 
 * @description: 
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
 * @description: 
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
 * @description: 
 * @param {ul_chain_t*} head
 * @param {uint8_t} act
 * @return {*}
 */
int ul_chain_act_set_status( ul_chain_t* head, uint8_t act ,ul_act_status sta  )
{
    int ret = SUCCESS;
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
 * @description: 
 * @param {ul_chain_t*} head
 * @return {*}
 */
ul_act_t* ul_act_list_get_valid_act( ul_chain_t* head )
{
    ul_chain_t* tmp;
    ul_act_t* detail = NULL;
    
    for ( tmp = head; tmp != NULL; tmp = tmp->next ) 
    {
        detail = head->detail;

        if ( detail->sta == UL_ACT_WAITING_ACK || detail->sta == UL_ACT_HOLDING ) {
            tmp = NULL;
            break;
        }

        if ( detail->sta == UL_ACT_QUEUING )
        {
            if( detail->flags & ( 1 << UL_ACT_FLAG_WAITACK ) )
            {
                detail->sta = UL_ACT_WAITING_ACK;
            }
            else
            {
                detail->sta = UL_ACT_FINISHED;
            }
            break;
        }
    }

    return detail;
}

/**
 * @function: 
 * @description: 
 * @param {ul_chain_t**} head
 * @param {ul_act_t*} act
 * @return {*}
 */
int ul_chain_act_callback( ul_chain_t** head, ul_act_t act )
{
    if ( act.callback != NULL ) {
        if ( act.sta == UL_ACT_FINISHED ) {
            act.callback( head, act.arg, SUCCESS ); //执行成功
        } else if ( act.sta == UL_ACT_TIMEOUT_TOO_MANY ) {
            act.callback( head, act.arg, TIMEOUT ); //超时
        } else if ( act.sta == UL_ACT_FAILED ) {
            act.callback( head, act.arg, INVAILD ); //执行错误
        }
    }
}

/**
 * @function: 
 * @description: 
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} end
 * @param {ul_chain_t*} node
 * @return {*}
 */
int ul_chain_act_try_remove( ul_chain_t** head, ul_chain_t** end, ul_chain_t* node )
{
    ul_act_t* act;
    int ret = NOVALUE;

    if( node && node->detail )
    {
        act = node->detail;

        if( !( act->flags & (1 << UL_ACT_FLAG_PERIODIC)) )
        {
            if( act->sta == UL_ACT_FINISHED || act->sta == UL_ACT_TIMEOUT_TOO_MANY || act->sta == UL_ACT_FAILED|| act->sta == UL_ACT_RESEND ){
                ul_chain_act_del( head, end, node);
                ul_chain_act_release( node );
                ret = SUCCESS;
            }
        }
    }
    
    return ret;
}


 /**
  * @function: 
  * @description: 
  * @param {bleh_act_t*} root
  * @return {*}
  */
int ul_chain_act_poll( ul_chain_t** head, ul_chain_t** end )
{
    int ret;
    ul_chain_t* tmp;
    ul_act_t* detail;

    for ( tmp = *head; tmp != NULL; tmp = tmp->next )
    {
        detail = tmp->detail;

        if( detail == NULL )
        {
            continue;
        }
        
        if( detail->sta == UL_ACT_WAITING_ACK )
        {
            if( detail->trig_time + detail->timeout_ms > 0 )
            {
                detail->delayed_cnt ++;
                if( detail->delayed_max <= detail->delayed_cnt )
                {
                    detail->sta = UL_ACT_TIMEOUT_TOO_MANY;
                }
                else
                {
                    //重传
                    detail->sta = UL_ACT_QUEUING;
                }
            }
        }

        ul_chain_act_callback( head, *detail );

        if( ul_chain_act_try_remove( head, end, tmp) == SUCCESS )
        {
            //清除成功后,tmp地址的内容被执行为NULL
            //这里是跳出操作
            //可以更换为手动更新tmp
            break;
        }
    }

    return ret;
}
