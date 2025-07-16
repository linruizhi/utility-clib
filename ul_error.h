/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-08-19 13:30:23
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-22 14:26:08
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_ERROR_H__
#define __UL_ERROR_H__

#define DATA_NORMAL         UL_ERR_SUCCESS   //数据正常
#define DATA_NEED_REP       1   //有数据需要回复
#define DATA_NO_INDICATE    2   //数据不需要指示

#define FIFO_SUCCESS        UL_ERR_SUCCESS 
#define FIFO_HANDLE_NULL    UL_ERR_INVAILD
#define FIFO_MALLOC_NULL    UL_ERR_NOMEMARY

typedef enum{
    UL_ERR_SUCCESS      =  0,  //成功/无错误
    UL_ERR_INVAILD      = -1,  //无效参数,参数不在有效范围内
    UL_ERR_NOMEMARY     = -2,  //内存不足
    UL_ERR_NOVALUE      = -3,  //未找到需求值
    UL_ERR_REPEATE      = -4,  //重复赋值,赋值前与赋值后的值相同
    UL_ERR_DATA_ERROR   = -5,  //输入数据有误
    UL_ERR_BUSY         = -6,  //繁忙
    UL_ERR_TIMEOUT      = -7,  //超时错误
    UL_ERR_FALSE        = -8,  //执行错误
    UL_ERR_STATUS_ERROR = -9,  //状态异常
}ul_errcode_t;

#endif // !__UL_ERROR_H__
