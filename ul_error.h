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


#define SUCCESS     0   //成功/无错误
#define INVAILD     -1  //无效参数,参数不在有效范围内
#define NOMEMARY    -2  //内存不足
#define NOVALUE     -3  //未找到需求值
#define REPEATE     -4  //重复赋值,赋值前与赋值后的值相同
#define DATA_ERROR  -5  //输入数据有误
#define BUSY        -6  //繁忙
#define TIMEOUT     -7  //超时错误


#define FIFO_SUCCESS        SUCCESS 
#define FIFO_HANDLE_NULL    INVAILD
#define FIFO_MALLOC_NULL    NOMEMARY

#endif // !__UL_ERROR_H__
