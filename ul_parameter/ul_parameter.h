#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "UTILITY_typedef/UTILITY_enum/UTILITY_enum.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/*
 * 参数存储地址
 */
#define PARAMETER_HEADER_ADDR NULL

#ifndef PARAMETER_HEADER_ADDR
	#error 参数头部存储地址未定义
#endif
/*
 * 参数信息结构体
 */
typedef struct{               
    uint8_t StartAddr;
    uint8_t length;
    operator_em em_operator;
	datalimit_em em_datalimit;
	uint16_t Mindata;
	uint16_t Maxdata;
    uint8_t *Name;
}ParameterTable;

/*
 * 参数头部信息结构体
 */
typedef struct{               
    uint32_t StartAddr; 		//参数起始地址
    uint32_t length;			//参数有效长度
    uint32_t CRC32;				//参数CRC32
}ParameterHeaderInfo;


/*
 * 参数结构体
 */
typedef struct{    
	//高4bit为大版本，低4bit为小版本，大版本基本不会发生改变
	uint8_t ParameterVersion;
	//设备名称,字符串
	uint8_t DevName[8];
	//设备唯一号
    uint8_t UniqueSN[8];
	//设备MAC地址
	uint8_t MACNumber[6];
	//设备模块号
    uint8_t ModuleID[2];
	//硬件版本
    uint8_t HWVersion[4];
	//软件版本
    uint8_t FWVersion[4];
}Parameter;

/* 参数偏移 */
enum em_parameter_t{
    em_UniqueSN = 1,
    em_ModuleID,
    em_HWVersion,
    em_FWVersion,
    em_MajorNumber,
    em_MinorNumber,
    em_AdvInterval,
    em_AdvTxPower,
    em_PassKey,
    em_MACNumber,
    em_EnableRF,
    em_RF433Channel,
    em_RF433Power,
    em_RF433DataRates,
    em_HeartBeatInterval,
    em_DFUEnable,
    em_EnableBT,
    em_BaudRate,
    em_EnableBTADV,
    PARAMETERSIZE,
};

/**
 * @brief Function for parameter_init
 * @details 参数表打印
 * @warning null 
 * @param[in] null
 * @return null
 */
void parameter_init( void );
/**
 * @brief Function for Read_Parameter
 * @details 读取参数
 * @warning null 
 * @param[offset] 参数偏移
 * @param[data] 参数数据
 * @param[length] 参数长度
 * @return null
 */
_Bool Read_Parameter( enum em_parameter_t offset , uint8_t* data , uint8_t length );
/**
 * @brief Function for Write_Parameter
 * @details 写参数
 * @warning null 
 * @param[offset] 参数偏移
 * @param[data] 参数数据
 * @param[length] 参数长度
 * @return null
 */
_Bool Write_Parameter( enum em_parameter_t offset , uint8_t* data , uint8_t length );
/**
 * @brief Function for Save_Parameter
 * @details 保存参数
 * @warning null 
 * @param[in] null
 * @return null
 */
void Save_Parameter( void );
/**
 * @brief Function for Get_ParameterReadOnly
 * @details 读取参数参数操作符
 * @warning null 
 * @param[offset] 参数偏移
 * @return 操作符
 */
uint8_t Get_ParameterReadOnly( enum em_parameter_t offset );
/**
 * @brief Function for Set_ParameterReadOnly
 * @details 设置参数参数操作符
 * @warning null 
 * @param[offset] 参数偏移
 * @param[ReadOnly] 操作权限
 * @return null
 */
void Set_ParameterReadOnly( enum em_parameter_t offset , uint8_t ReadOnly );
/**
 * @brief Function for Define_Parameter
 * @details 初始化参数
 * @warning null 
 * @param[in]  null
 * @return null
 */
void Define_Parameter( void );
/**
 * @brief Function for Get_ParameterName
 * @details 获取参数名称
 * @warning null 
 * @param[offset] 参数偏移
 * @return null
 */
uint8_t* Get_ParameterName( enum em_parameter_t offset );

#endif //__PARAMETER_H__


