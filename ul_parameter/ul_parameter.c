#include "UTILITY_parameter.h"

/*
* 静态局部变量，参数值
*/
static Parameter stAllparameter;
static ParameterHeaderInfo stParameterHeader;
/*
 * 默认参数表
 */
const Parameter stDefPara=
{
    01,
    "MyDevice",
    {0x80,0x01,0x05,0x37,0xFF,0xFF,0xFF,0xFF},  //SN
    {0x05,0x37,0xFF,0xFF,0xFF,0xFF},            //MAC
    {0xFF,0xFF},                                //ModuleID
    {0x20,0x05,0x25,0x01},                      //Hw version
    {0x22,0x12,0x15,0x01},                      //FW Version
};

/*
 * 参数信息表
 */
ParameterTable ParameterQueryTable[PARAMETERSIZE+1]=
{
    {0x00,    1,    operator_readandwrite,    limit_none,  0,0,   (uint8_t*)"ParameterVersion"},
    {0x14,    8,    operator_readandwrite,    limit_none,  0,0,  (uint8_t*)"DevName"},     
    {0x00,    8,    operator_readandwrite,    limit_none,  0,0,  (uint8_t*)"UniqueSN"},
    {0x1E,    6,    operator_read,            limit_none,  0,0,  (uint8_t*)"MACNumber"},
    {0x08,    2,    operator_readandwrite,    limit_none,  0,0,  (uint8_t*)"ModuleID"},        //2
    {0x0A,    4,    operator_read,            limit_none,  0,0,  (uint8_t*)"HWVersion"},     //3
    {0x0E,    4,    operator_read,            limit_none,  0,0,  (uint8_t*)"FWVersion"},     //4
    // {0x12,    2,    operator_readandwrite,      (uint8_t*)"MajorNumber"},     //5
    // {0x14,    2,    operator_readandwrite,      (uint8_t*)"MinorNumber"},     //6
    // {0x16,    1,    operator_readandwrite,      (uint8_t*)"AdvInterval"},     //7
    // {0x17,    1,    operator_readandwrite,      (uint8_t*)"AdvTxPower"},     //8
    // {0x18,    6,    operator_readandwrite,      (uint8_t*)"PassKey"},         //9
    // {0x1E,    6,    operator_read,              (uint8_t*)"MACNumber"},     //A
    // {0x24,    1,    operator_readandwrite,      (uint8_t*)"EnableRF"},         //B
    // {0x25,    1,    operator_readandwrite,      (uint8_t*)"RF433_Channel"}, //C
    // {0x26,    1,    operator_readandwrite,      (uint8_t*)"RF433_OutputPower"},//D
    // {0x27,    1,    operator_readandwrite,      (uint8_t*)"RF433_DataRates"},        //E
    // {0x28,    2,    operator_readandwrite,      (uint8_t*)"HeartBeatInterval"},//F
    // {0x2A,    1,    operator_readandwrite,      (uint8_t*)"DFUEnable"},    //10

    // {0x2B,    1,    operator_readandwrite,      (uint8_t*)"EnableBT"},         //11
    // {0x2C,    1,    operator_readandwrite,      (uint8_t*)"Baud_Rate"},        //12
    // {0x2D,    1,    operator_readandwrite,      (uint8_t*)"EnableBTADV"},    //13

    // {0x2E,    2,    operator_readandwrite,      (uint8_t*)"Resave"},        //0x14
};
/**
 * @brief Function for parameter_init
 * @details 参数初始化
 * @warning null
 * @param[in]  null
 * @return null
 */
void parameter_init( void )
{
    uint8_t Parameter_length = sizeof( Parameter );
    uint32_t crc32 = 0;
    /* 更新参数信息表，参数信息表首参数地址偏移必须为0 */
    if( ParameterQueryTable[0].StartAddr != 0 )
    {
        #error 首参数地址偏移不为0
    }
    for( int i=1;i<PARAMETERSIZE;i++ )
    {
        ParameterQueryTable[i].StartAddr = ParameterQueryTable[i-1].StartAddr+ParameterQueryTable[i].length;
    }
    /* 初始化默认参数 */
    memcpy( stAllparameter.ParameterVersion, stDefPara.ParameterVersion, Parameter_length );
    /* 读取参数头部信息 */
    FlashReadData( PARAMETER_HEADER_ADDR , stParameterHeader.StartAddr , 24 );
    /* 读取参数信息 */
    FlashReadData( stParameterHeader.StartAddr , stAllparameter.UniqueSN , stParameterHeader.length );
    /* 参数CRC校验 */
    if( crc32 != stParameterHeader.CRC32 )//CRC校验失败,Flash可能存在问题
    {
        /* 放弃使用Flash参数，改为默认参数 */
        memcpy( stAllparameter.ParameterVersion, stDefPara.ParameterVersion, Parameter_length );
        /* 修改头部信息 */
        stParameterHeader.CRC32 = crc32;
        stParameterHeader.length = Parameter_length;
        /* 存储头部信息 */
        #warning 未存储
    }
}
/**
 * @brief Function for Define_Parameter
 * @details 恢复默认参数
 * @warning null
 * @param[in]  null
 * @return null
 */
void Define_Parameter( void )
{
    uint8_t Parameter_length = sizeof(Parameter);
    memcpy( stAllparameter.UniqueSN , stDefPara.UniqueSN , Parameter_length );
    NRF_LOG_INFO("Parameter use default values");
}
/**
 * @brief Function for Read_Parameter
 * @details 读取参数
 * @warning null
 * @param[in]  null
 * @return null
 */
_Bool Read_Parameter( enum em_parameter_t offset , uint8_t* data , uint8_t length )
{
    _Bool result = 0;
    if( length == ParameterQueryTable[offset].length )
    {
        memcpy( data , stAllparameter.UniqueSN + ParameterQueryTable[offset].StartAddr ,length );
        result = 1;
    }
    return result;
}

/**
 * @brief Function for Save_Parameter
 * @details 保存参数，Flash需要先擦除再写入
 * @warning null
 * @param[in]  null
 * @return null
 */
void Save_Parameter( void )
{
    uint8_t* data = stAllparameter.ParameterVersion;
    uint32_t ParameterSize = MEM_ALIGN(sizeof(Parameter),4);//Flash写入需要4字节对齐
    #error 此处未保存参数
    // ErasePage( FS_START_ADDR , 1 );
    // FlashWritedata( FS_START_ADDR , data , sizeof(Parameter));
}

/**
 * @brief Function for Write_Parameter
 * @details 写参数
 * @warning null
 * @param[in]  null
 * @return null
 */
_Bool Write_Parameter( enum em_parameter_t offset , uint8_t* data , uint8_t length )
{
    _Bool result = 0;
    /* 函数长度比较 */
    if( length == ParameterQueryTable[offset].length )
    {
        memcpy( stAllparameter.UniqueSN + ParameterQueryTable[offset].StartAddr , data ,length );
        result = 1;
    }
    return result;
}
/**
 * @brief Function for Get_ParameterReadOnly
 * @details 获取只读参数值
 * @warning null
 * @param[offset] 参数偏移
 * @return 参数值
 */
uint8_t Get_ParameterReadOnly( enum em_parameter_t offset )
{
    return ParameterQueryTable[offset].em_operator;
}
/**
 * @brief Function for Set_ParameterReadOnly
 * @details 设置只读参数值
 * @warning null
 * @param[offset] 参数偏移
 * @param[em_operator] 新的值
 * @return null
 */
void Set_ParameterReadOnly( enum em_parameter_t offset , uint8_t em_operator )
{
    ParameterQueryTable[offset].em_operator = em_operator;
}

/**
 * @brief Function for Get_ParameterReadOnly
 * @details 获取只读参数值
 * @warning null
 * @param[offset] 参数偏移
 * @return 参数值
 */
uint8_t* Get_ParameterName( enum em_parameter_t offset )
{
    return ParameterQueryTable[offset].Name;
}

