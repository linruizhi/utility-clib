#ifndef _AWARE_TECHNOLOGY_LIBARCH_H__
#define _AWARE_TECHNOLOGY_LIBARCH_H__

/* c语言标准预定义 */
#define UTILITY_LIB_PARAMETER_MANAGEMENT    0   /* 配置是否使用通用参数管理 */
#define UTILITY_LIB_CRC                     1   /* 配置是否使用通用CRC */

#define UTILITY_LIB_GENERAL_FUNCTION        1   /* 配置是否启用通用函数 */

#define UTILITY_SYSTEM_USE_OS               0   /* 配置是否选用操作系统 */
#define UTILITY_SYSTEM_ARCH                 1   /* 架构选择.0：单片机，1：Linux */

/* 标准头文件 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"

#include "ul_error.h"

#include "libarch_os.h"
#include "libarch_linux.h"

void utility_lib_Init( void );

#endif //_AWARE_TECHNOLOGY_LIBARCH_H
