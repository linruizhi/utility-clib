/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-21 10:51:55
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-21 11:38:30
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#include <string.h>
#include "vprint.h"
#include "ul_file.h"

/**
 * @function: 
 * @description: 
 * @param {char*} file_path
 * @return {*}
 */
int ul_file_exist_check( const char* file_path )
{
    int ret;

    ret = access( file_path, F_OK );
    if( ret == -1 )
    {
        ul_printf( LOG_ERROR, "file <%s> no exist", file_path);
    }

    return ret;
}

/**
 * @function: 
 * @description: 
 * @param {char*} file_path
 * @param {char*} modes
 * @return {*}
 */
FILE * ul_file_open( const char* file_path ,char* modes )
{
    FILE *fp;

    fp = fopen( file_path, modes );
    if( fp == NULL )
    {
        ul_printf( LOG_ERROR, "file <%s> not open", file_path);
    }

    return fp;
}

/**
 * @function: 
 * @description: 
 * @param {FILE *} file
 * @return {*}
 */
int ul_file_close( FILE * file )
{
    int ret;

    ret = fclose( file );
    if( ret != 0 ){
        ul_printf( LOG_ERROR, "file close error[%d]",ret);
    }

    return ret;
}

/**
 * @function: 
 * @description: 读取偏移位置的一行. 循环调用时,需要关注是否对文件偏移进行手动修改
 * @param {FILE} *file
 * @param {char} *buff
 * @return {*}
 */
int ul_file_read_line( FILE * file, char* buffer, int buffer_size )
{
    int ret = UL_FILE_READ_OVER;
    char *newline_pos;

    if( fgets( buffer, buffer_size, file ) != NULL )
    {
        newline_pos = strchr( buffer, '\n' );

        if( newline_pos != NULL )
        {
            ret = UL_FILE_READ_OK;
        }
        else
        {
            //判断是否时文件末尾
            if(feof(file) )
            {
                ret = UL_FILE_READ_LAST_LINE_OK;
            }
            else
            {
                ret = UL_FILE_READ_PARTIAL;
            }
        }
    }

    return ret;
}

/**
 * @function: 
 * @description: 
 * @param {FILE *} file
 * @param {char*} buffer
 * @return {*}
 */
int ul_file_write_line( FILE * file, const char* buffer )
{
    int ret = UL_FILE_WRITE_OK;

    ret = fputs( buffer, file );

    if( ret == EOF )
    {
        ret = UL_FILE_WRITE_ERROR;
    }

    return ret;
}