/*
 * @Author: linruizhi
 * @Description:
 * @Date: 2024-11-22 10:33:30
 * @LastEditors: linruizhi
 * @LastEditTime: 2025-01-03 16:08:30
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved.
 */
// #include "vprint.h"
#include "ul_chain_act.h"

/**
 * @function:
 * @description:
 * @return {*}
 */
ul_chain_t *ul_chain_act_create(ul_act_attr_t attr)
{
    ul_chain_t *node;

    node = ul_chain_create();
    if (node != NULL)
    {
        node->detail = libc_malloc(sizeof(ul_act_t));
        if (node->detail != NULL)
        {
            ((ul_act_t *)node->detail)->act = attr.act;
            ((ul_act_t *)node->detail)->data = attr.data;
            ((ul_act_t *)node->detail)->len = attr.len;
            ((ul_act_t *)node->detail)->flags = attr.flags;
            ((ul_act_t *)node->detail)->timeout_ms = attr.timeout_ms;
            ((ul_act_t *)node->detail)->delayed_max = attr.delayed_max;
            ((ul_act_t *)node->detail)->delayed_cnt = 0;
            ((ul_act_t *)node->detail)->callback = attr.callback;
            ((ul_act_t *)node->detail)->sta = UL_ACT_QUEUING;
            ((ul_act_t *)node->detail)->arg = attr.arg;
        }
        else
        {
            ul_chain_release(node);
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
int ul_chain_act_release(ul_chain_t *node)
{
    ul_act_t *detail;

    if (node != NULL)
    {
        if (node->detail != NULL)
        {
            detail = node->detail;
            if (detail->data != NULL)
            {
                libc_free(detail->data);
                detail->data = NULL;
            }
            if (detail->arg != NULL)
            {
                libc_free(detail->arg);
                detail->arg = NULL;
            }
            libc_free(detail);
            node->detail = NULL;
        }

        libc_free(node);
    }

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
int ul_chain_act_add(ul_act_chain_t *chain, ul_chain_t *node)
{
    return ul_chain_add(&chain->head, &chain->tail, node);
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} tail
 * @param {ul_chain_t*} node
 * @return {*}
 */
static int ul_chain_act_del(ul_act_chain_t *chain, ul_chain_t *node)
{
    return ul_chain_del(&chain->head, &chain->tail, node);
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t*} head
 * @param {uint8_t} act
 * @return {*}
 */
ul_errcode_t ul_chain_act_check_existed(ul_chain_t *head, uint8_t act)
{
    ul_errcode_t ret = UL_ERR_NOVALUE;
    ul_chain_t *tmp;
    ul_act_t *detail;

    for (tmp = head; tmp != NULL; tmp = tmp->next)
    {
        detail = tmp->detail;
        if (detail != NULL)
        {
            if (detail->act == act)
            {
                ret = UL_ERR_SUCCESS;
                break;
            }
        }
        else
        {
            ret = UL_ERR_INVAILD;
        }
    }

    return ret;
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t*} head
 * @param {uint8_t} act
 * @return {*}
 */
int ul_chain_act_set_status(ul_chain_t *head, uint8_t act, ul_act_status sta)
{
    int ret = UL_ERR_NOVALUE;
    ul_chain_t *tmp;
    ul_act_t *detail;

    for (tmp = head; tmp != NULL; tmp = tmp->next)
    {
        detail = tmp->detail;
        if (detail != NULL)
        {
            if (detail->act == act)
            {
                detail->sta = sta;
                detail->delayed_cnt = 0;
                ret = UL_ERR_SUCCESS;
                break;
            }
        }
        else
        {
            ret = UL_ERR_INVAILD;
        }
    }

    return ret;
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t*} head
 * @param {uint8_t} act
 * @param {ul_act_status} sta
 * @return {*}
 */
ul_errcode_t ul_chain_act_get_arg(ul_chain_t *head, uint8_t act, void **arg)
{
    ul_errcode_t ret = UL_ERR_NOVALUE;
    ul_chain_t *tmp;
    ul_act_t *detail;

    for (tmp = head; tmp != NULL; tmp = tmp->next)
    {
        detail = tmp->detail;
        if (detail != NULL)
        {
            if (detail->act == act)
            {
                *arg = detail->arg;

                ret = UL_ERR_SUCCESS;
                break;
            }
        }
        else
        {
            ret = UL_ERR_INVAILD;
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
ul_act_t *ul_act_list_get_valid_act(ul_act_chain_t chain, uint64_t time_ms, int *ret)
{
    ul_chain_t *tmp;
    ul_act_t *detail = NULL;

    for (tmp = chain.head; tmp != NULL; tmp = tmp->next)
    {
        if (((ul_act_t *)tmp->detail)->sta == UL_ACT_WAITING_ACK)
        {
            detail = NULL;
            *ret = UL_ERR_BUSY;
            break;
        }

        if (((ul_act_t *)tmp->detail)->sta == UL_ACT_QUEUING)
        {
            if (((ul_act_t *)tmp->detail)->flags & (1 << UL_ACT_FLAG_WAITACK))
            {
                ((ul_act_t *)tmp->detail)->sta = UL_ACT_WAITING_ACK;
            }
            else if (((ul_act_t *)tmp->detail)->flags & (1 << UL_ACT_FLAG_PERIODIC))
            {
                ((ul_act_t *)tmp->detail)->sta = UL_ACT_DELAYED; // 等待下个周期
            }
            else
            {
                ((ul_act_t *)tmp->detail)->sta = UL_ACT_FINISHED;
            }
            // 调整触发时间
            ((ul_act_t *)tmp->detail)->trig_time = time_ms;

            detail = tmp->detail;
            *ret = UL_ERR_BUSY;
            break;
        }
    }

    *ret = UL_ERR_NOVALUE;

    return detail;
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t**} head
 * @param {ul_act_t*} act
 * @return {*}
 */
static int ul_chain_act_callback(ul_act_chain_t *chain, ul_act_t *act)
{
    if (act->callback)
    {
        act->callback(chain, act); // 执行成功
    }
    return UL_ERR_SUCCESS;
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t**} head
 * @param {ul_chain_t**} end
 * @param {ul_chain_t*} node
 * @return {*}
 */
static int ul_chain_act_try_remove(ul_act_chain_t *chain, ul_chain_t *node)
{
    ul_act_t *act;
    int ret = UL_ERR_NOVALUE;

    if (node && node->detail)
    {
        act = node->detail;

        switch (act->sta)
        {
        case UL_ACT_FINISHED:
        case UL_ACT_TIMEOUT_TOO_MANY:
        case UL_ACT_FAILED:
        case UL_ACT_ABANDON:
            if (act->flags & (1 << UL_ACT_FLAG_PERIODIC))
            {
                ret = UL_ERR_FALSE; // 循环指令，禁止删除
            }
            else
            {
                ret = ul_chain_act_del(chain, node);
                ul_chain_act_release(node);
            }
            break;
        default:
            ret = UL_ERR_STATUS_ERROR;
            break;
        }
    }

    return ret;
}

/**
 * @function:
 * @description:
 * @return {*}
 */
int ul_chain_act_set_all_pause(ul_act_chain_t chain)
{
    int ret = UL_ERR_SUCCESS;
    ul_chain_t *tmp;

    for (tmp = chain.head; tmp != NULL; tmp = tmp->next)
    {
        if ((ul_act_t *)tmp->detail == NULL) // 异常
        {
            continue;
        }

        ((ul_act_t *)tmp->detail)->sta = UL_ACT_PAUSING;
    }
    return ret;
}

/**
 * @function:
 * @description:
 * @param {ul_chain_t**} head
 * @return {*}
 */
int ul_chain_act_set_all_resume(ul_act_chain_t chain)
{
    int ret = UL_ERR_SUCCESS;
    ul_chain_t *tmp;

    for (tmp = chain.head; tmp != NULL; tmp = tmp->next)
    {
        if ((ul_act_t *)tmp->detail == NULL) // 异常
        {
            // ul_printf( LOG_ERROR, "chain node detail is NULL" );
            continue;
        }

        ((ul_act_t *)tmp->detail)->sta = UL_ACT_QUEUING;
    }
    return ret;
}

/**
 * @function:
 * @description:
 * @param {bleh_act_t*} root
 * @return {*}
 */
int ul_chain_act_poll(ul_act_chain_t *chain, uint64_t time_ms)
{
    int ret = UL_ERR_NOVALUE, i = 0;
    ul_chain_t *tmp;
    // ul_act_t* act;

    for (tmp = chain->head; tmp != NULL; tmp = tmp->next)
    {
        i++;
        if ((ul_act_t *)tmp->detail == NULL) // 异常
        {
            continue;
        }

        // 该指令被暂停
        if (((ul_act_t *)tmp->detail)->sta == UL_ACT_PAUSING)
        {
            continue;
        }

        ret = UL_ERR_BUSY;

        // ul_printf(LOG_DEBUG, "chain poll now act id[%d],sta[%d],act[%x],arg[%d]", i, ((ul_act_t*)tmp->detail)->sta, ((ul_act_t*)tmp->detail)->act, ((ul_act_t*)tmp->detail)->arg );

        if (((ul_act_t *)tmp->detail)->sta == UL_ACT_DELAYED)
        {
            if (time_ms - ((ul_act_t *)tmp->detail)->trig_time >= ((ul_act_t *)tmp->detail)->timeout_ms) // 判断超时
            {
                ((ul_act_t *)tmp->detail)->sta = UL_ACT_QUEUING;
                // ul_printf(LOG_DEBUG,"chain[%d] node[%d] act[%d] timeout,resend", chain, tmp, ((ul_act_t*)tmp->detail)->act );
            }
        }

        if (((ul_act_t *)tmp->detail)->sta == UL_ACT_WAITING_ACK)
        {
            if (time_ms - ((ul_act_t *)tmp->detail)->trig_time >= ((ul_act_t *)tmp->detail)->timeout_ms) // 判断超时
            {
                ((ul_act_t *)tmp->detail)->delayed_cnt++;
                if (((ul_act_t *)tmp->detail)->flags & (1 << UL_ACT_FLAG_MUSTOK) ||
                    ((ul_act_t *)tmp->detail)->delayed_max >= ((ul_act_t *)tmp->detail)->delayed_cnt)
                {
                    // 重传
                    ((ul_act_t *)tmp->detail)->sta = UL_ACT_QUEUING;
                    // ul_printf(LOG_DEBUG, "node[%d] act[%x] arg[%d] timeout resend", tmp, ((ul_act_t*)tmp->detail)->act, ((ul_act_t*)tmp->detail)->arg );
                }
                else
                {
                    ((ul_act_t *)tmp->detail)->sta = UL_ACT_TIMEOUT_TOO_MANY;
                    printf("node[%d] act[%x] timeout too many[%d/%d]", tmp, ((ul_act_t *)tmp->detail)->act, ((ul_act_t *)tmp->detail)->delayed_cnt, ((ul_act_t *)tmp->detail)->delayed_max);
                }
            }
        }

        ul_chain_act_callback(chain, (ul_act_t *)tmp->detail);

        ret = ul_chain_act_try_remove(chain, tmp);
        if (ret == UL_ERR_SUCCESS)
        {
            // ul_printf(LOG_DEBUG, "chain[%d] remove a node[%d]", chain, tmp );
            // 清除成功后,tmp地址的内容被执行为NULL
            // 这里是跳出操作,但是可以手动更新tmp的值使得继续处理
            break;
        }
    }

    return ret;
}
