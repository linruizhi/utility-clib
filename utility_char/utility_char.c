
/* 文档包含对字符操作的函数接口 */
/* 如string字符大小写字符转换、字符翻转等 */


#include "utilty_char.h"
/*
*********************************************************************************************************
*	函 数 名: swap
*	功能说明: 交换一字节数据
*	形    参: a,b
*	返 回 值: 无
*********************************************************************************************************
*/
void swap(char *a, char *b) {
    char t = *a;
    *a = *b, *b = t;
}

/*
*********************************************************************************************************
*	函 数 名: reverseString
*	功能说明: 反转数据头尾，对象：字符串/数组
*	形    参: 数据地址,数据大小
*	返 回 值: 无
*********************************************************************************************************
*/
void reverseString(char *s, int sSize) {
    for (int left = 0, right = sSize - 1; left < right; ++left, --right) {
        swap(s + left, s + right);
    }
}

/*
*********************************************************************************************************
*	函 数 名: htoa
*	功能说明: hex数据转ASCII码
*	形    参: ASCII数据地址,hex数据地址,ASCII数据长度
*	返 回 值: hex长度
*********************************************************************************************************
*/
int htoa(const unsigned char* hex, unsigned char* asc, int hex_len )
{
    int i = 0;
    int cnt;
    int tot_len = 0;

    if (  hex == 0 || asc == 0  ) {
        return tot_len;
    }
    tot_len = hex_len*2;
    for ( i = 0; i < hex_len; i++ ) {
        cnt = ( hex[i] >> 4 );
        if ( cnt > 9 ) {
            asc[2 * i] =  cnt + 65 - 0x0A;
        } else {
            asc[2 * i] =  cnt + 0x30;
        }

        cnt = ( hex[i] & 0x0F );
        if ( cnt > 9 ) {
            asc[2 * i + 1] =  cnt + 65 - 0x0A;
        } else {
            asc[2 * i + 1] =  cnt + 0x30;
        }
    }
    return tot_len;
}

/*
*********************************************************************************************************
*	函 数 名: atoh
*	功能说明: ASCII码转hex数据
*	形    参: ASCII数据地址,hex数据地址,ASCII数据长度
*	返 回 值: hex长度
*********************************************************************************************************
*/
int atoh( const unsigned char* asc,unsigned char* hex, int asc_len )
{
    char temp;
    int i;

    if ( hex == 0 || asc == 0 ) {
        return -1;
    }
    asc_len /= 2;
    for ( i = 0; i < asc_len; i++ )
    {
        temp = asc[2 * i];
        if ( temp >= '0' && temp <= '9' ) {
            hex[i] = ( ( temp - '0' ) << 4 );
        } else if ( temp >= 'A' && temp <= 'F' ) {
            hex[i] = ( ( temp - '7' ) << 4 );
        } else if ( temp >= 'a' && temp <= 'f' ) {
            hex[i] = ( ( temp - 'W' ) << 4 );
        }
        
        temp = asc[2 * i + 1];
        if ( temp >= '0' && temp <= '9' ) {
            hex[i] += ( temp - '0' );
        } else if( temp >= 'A' && temp <= 'F' ) {
            hex[i] += ( temp - '7' );
        } else {
            hex[i] += ( temp - 'W' );
        }
    }
    return asc_len;
}


