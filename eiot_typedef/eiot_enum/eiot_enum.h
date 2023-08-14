#ifndef __EIOT_ENUM_H__
#define __EIOT_ENUM_H__

/* 数据操作权限,按位操作 */
typedef enum
{
    operator_none = 0,
    operator_read = 1,
    operator_write = 2,
    operator_readandwrite = 3,
}operator_em;

/* 数据限制模式 */
typedef enum
{
    limit_none = 0,     //无限制
    limit_range = 1,    //范围限制
    limit_confirm = 2,  //确认值限制
}datalimit_em;


#endif //__EIOT_ENUM_H__
