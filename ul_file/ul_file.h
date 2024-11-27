/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-21 10:52:12
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-21 11:39:06
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#ifndef __UL_FILE_H__
#define __UL_FILE_H__

#include <stdio.h>
#include <unistd.h>

#define UL_FILE_WRITE_ERROR -1
#define UL_FILE_WRITE_OK 0

#define UL_FILE_READ_LAST_LINE_OK 1
#define UL_FILE_READ_OK 0
#define UL_FILE_READ_PARTIAL -1
#define UL_FILE_READ_OVER -2

int ul_file_read_line( FILE * file, char* buffer, int buffer_size );
int ul_file_write_line( FILE * file, const char* buffer );
int ul_file_write_bytes( FILE * file, const void* buffer, int size );
int ul_file_read_bytes( FILE * file, void* buffer, int size );

int ul_file_exist_check( const char* file_path );
FILE * ul_file_open( const char* file_path ,char* modes );
int ul_file_close( FILE * file );


#endif // !__UL_FILE_H__