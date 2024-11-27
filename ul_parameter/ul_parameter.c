#include "UTILITY_parameter.h"

/*
* ��̬�ֲ�����������ֵ
*/
static Parameter stAllparameter;
static ParameterHeaderInfo stParameterHeader;
/*
 * Ĭ�ϲ�����
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
 * ������Ϣ��
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
 * @details ������ʼ��
 * @warning null
 * @param[in]  null
 * @return null
 */
void parameter_init( void )
{
    uint8_t Parameter_length = sizeof( Parameter );
    uint32_t crc32 = 0;
    /* ���²�����Ϣ��������Ϣ���ײ�����ַƫ�Ʊ���Ϊ0 */
    if( ParameterQueryTable[0].StartAddr != 0 )
    {
        #error �ײ�����ַƫ�Ʋ�Ϊ0
    }
    for( int i=1;i<PARAMETERSIZE;i++ )
    {
        ParameterQueryTable[i].StartAddr = ParameterQueryTable[i-1].StartAddr+ParameterQueryTable[i].length;
    }
    /* ��ʼ��Ĭ�ϲ��� */
    memcpy( stAllparameter.ParameterVersion, stDefPara.ParameterVersion, Parameter_length );
    /* ��ȡ����ͷ����Ϣ */
    FlashReadData( PARAMETER_HEADER_ADDR , stParameterHeader.StartAddr , 24 );
    /* ��ȡ������Ϣ */
    FlashReadData( stParameterHeader.StartAddr , stAllparameter.UniqueSN , stParameterHeader.length );
    /* ����CRCУ�� */
    if( crc32 != stParameterHeader.CRC32 )//CRCУ��ʧ��,Flash���ܴ�������
    {
        /* ����ʹ��Flash��������ΪĬ�ϲ��� */
        memcpy( stAllparameter.ParameterVersion, stDefPara.ParameterVersion, Parameter_length );
        /* �޸�ͷ����Ϣ */
        stParameterHeader.CRC32 = crc32;
        stParameterHeader.length = Parameter_length;
        /* �洢ͷ����Ϣ */
        #warning δ�洢
    }
}
/**
 * @brief Function for Define_Parameter
 * @details �ָ�Ĭ�ϲ���
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
 * @details ��ȡ����
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
 * @details ���������Flash��Ҫ�Ȳ�����д��
 * @warning null
 * @param[in]  null
 * @return null
 */
void Save_Parameter( void )
{
    uint8_t* data = stAllparameter.ParameterVersion;
    uint32_t ParameterSize = MEM_ALIGN(sizeof(Parameter),4);//Flashд����Ҫ4�ֽڶ���
    #error �˴�δ�������
    // ErasePage( FS_START_ADDR , 1 );
    // FlashWritedata( FS_START_ADDR , data , sizeof(Parameter));
}

/**
 * @brief Function for Write_Parameter
 * @details д����
 * @warning null
 * @param[in]  null
 * @return null
 */
_Bool Write_Parameter( enum em_parameter_t offset , uint8_t* data , uint8_t length )
{
    _Bool result = 0;
    /* �������ȱȽ� */
    if( length == ParameterQueryTable[offset].length )
    {
        memcpy( stAllparameter.UniqueSN + ParameterQueryTable[offset].StartAddr , data ,length );
        result = 1;
    }
    return result;
}
/**
 * @brief Function for Get_ParameterReadOnly
 * @details ��ȡֻ������ֵ
 * @warning null
 * @param[offset] ����ƫ��
 * @return ����ֵ
 */
uint8_t Get_ParameterReadOnly( enum em_parameter_t offset )
{
    return ParameterQueryTable[offset].em_operator;
}
/**
 * @brief Function for Set_ParameterReadOnly
 * @details ����ֻ������ֵ
 * @warning null
 * @param[offset] ����ƫ��
 * @param[em_operator] �µ�ֵ
 * @return null
 */
void Set_ParameterReadOnly( enum em_parameter_t offset , uint8_t em_operator )
{
    ParameterQueryTable[offset].em_operator = em_operator;
}

/**
 * @brief Function for Get_ParameterReadOnly
 * @details ��ȡֻ������ֵ
 * @warning null
 * @param[offset] ����ƫ��
 * @return ����ֵ
 */
uint8_t* Get_ParameterName( enum em_parameter_t offset )
{
    return ParameterQueryTable[offset].Name;
}

