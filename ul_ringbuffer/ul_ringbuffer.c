/*
 * @Author: linruizhi
 * @Description: 
 * @Date: 2024-11-22 17:17:33
 * @LastEditors: linruizhi
 * @LastEditTime: 2024-11-22 18:21:21
 * @Copyright: Copyright (c) 2024 by Awaretec Co., Ltd., All Rights Reserved. 
 */
#include <stdio.h>
#include <stdint.h>
#include "UTILITY_fifo.h"
#include "ul_error.h"
#include "ul_ringbuffer.h"

/**
 * @function: 
 * @description: 
 * @return {*}
 */
ul_ringbuffer_t* ul_ringbuffer_create( ul_rb_attr_t attr  )
{
    ul_ringbuffer_t* rb = NULL;
    uint32_t size;

    if( attr.element_size && attr.capacity )
    {
        if( attr.buff == NULL )
        {
            attr.buff = libc_malloc( attr.element_size*attr.capacity );
        }

        if( attr.buff )
        {
            rb = libc_malloc( sizeof( ul_ringbuffer_t ) );
            if( rb == NULL )
            {
                libc_free( attr.buff );
            }
        }

        if( rb )
        {
            rb->buff = attr.buff;
            rb->buff = attr.capacity;
            rb->element_size = attr.element_size;
            rb->head = rb->tail = rb->buff;
        }
    }

    return rb;
}

/**
 * @function: 
 * @description: 
 * @param {ul_ringbuffer_t *} rb
 * @return {*}
 */
int ul_ringbuffer_init( ul_ringbuffer_t *rb, ul_rb_attr_t attr )
{
    int ret = INVAILD;

    if( rb && attr.element_size && attr.capacity )
    {
        if( attr.buff == NULL )
        {
            attr.buff = libc_malloc( attr.element_size*attr.capacity );
        }

        if( attr.buff )
        {
            rb->buff = attr.buff;
            rb->buff = attr.capacity;
            rb->element_size = attr.element_size;
            rb->head = rb->tail = rb->buff;

            ret = SUCCESS;
        }
        else
        {
            ret = NOMEMARY;
        }
    }

    return ret;
}

/**
 * @function: 
 * @description: 
 * @param {ul_ringbuffer_t} *rb
 * @return {*}
 */
int ul_ringbuffer_get_element_number( ul_ringbuffer_t *rb )
{
    int num;
    
    if ( rb->head == rb->tail )
    {
        num = 0;
    }
    else if( rb->head < rb->tail )
    {
        num = rb->tail - rb->head;
        num = num / rb->element_size;
    }
    else{
        num = rb->capacity - (rb->head - rb->tail);
        num = num / rb->element_size;
    }

    return num;
}

/**
 * @function: 
 * @description: 
 * @param {ul_ringbuffer_t} *rb
 * @param {uint8_t} count
 * @return {*}
 */
int ul_ringbuffer_del_elements( ul_ringbuffer_t *rb, uint8_t count )
{
    int ret,delSz;
    
    if( rb->head == rb->tail )
    {
        ret = INVAILD;
    }
    else
    {
        delSz = count*rb->element_size;
        
        if( rb->head + delSz > rb->capacity + rb->buff )
        {
            rb->head = rb->head + delSz - rb->capacity;
        }
        else
        {
            rb->head = rb->head + delSz;
        }
    }

    return SUCCESS;
}


int32_t ul_ringbuffer_add_elements( ul_ringbuffer_t *rb, void* elems, uint8_t count )
{
    int copySz = 0,ret,all_size,lastSz,freeSz;

    if( elems == NULL || rb == NULL )
        return INVAILD;

    all_size = count * rb->element_size;

    /* 头指针小于尾指针 */
    if( rb->head < rb->tail )
    {
        freeSz = rb->tail - rb->head;
        memcpy( elems, rb->head, freeSz ); //写入空白区域

        //覆盖原始数据
        //移动写指针
        if( freeSz < all_size ) //将会覆盖部分数据
        {
            ul_ringbuffer_del_elements( rb, 1 ); //清除一部分空白
        }
        copySz = min( all_size, rb->tail - rb->head );
        memcpy( elems, rb->head, copySz );
    }
    else if( rb->head == rb->tail )
    {
        copySz = 0;
    }
    else
    {
        if( count < rb->capacity-(rb->head - rb->buff))  /* 读取的比数据到结尾的要少 */
        {
            copySz = count;
            memcpy(data, rb->head, copySz);
        }
        /* rb需要从尾转到头 */
        else 
        {
            copySz = rb->capacity - (rb->head - rb->buff);
            memcpy(data, rb->head, copySz);
            count = min(count-copySz,(rb->tail-rb->buff));
            memcpy((uint8_t*)data+copySz,rb->buff,count);
            copySz+=count;
        }
    }
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }
    return copySz;	
}

int32_t ul_ringbuffer_add_element( ul_ringbuffer_t *rb, void* elem )
{
    int num,ret;

    ret = rbNotNULL(rb);
    if(ret!=0)
    {
        return ret;
    }

    RB_LOCK( rb );
    if ( rb->head == rb->tail )
    {
        num = rb->capacity;
    }else if (rb->head < rb->tail)	
    {
        num = rb->capacity - (rb->tail - rb->head);
    }else{
        num = rb->head - rb->tail;
    }
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }
    
    return num;
}


int32_t capacity(rb_t *rb)
{
    int num,ret;

    ret = rbNotNULL(rb);
    if(ret!=0)
    {
        return ret;
    }

    RB_LOCK( rb );
    num = rb->capacity;
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }

    return num;
}

/*
*********************************************************************************************************
*	函 数 名: rbNotNULL
*	功能说明: rb不为空
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
static int rbNotNULL( rb_t* rb )
{
    if(NULL == rb)
    {
        #ifdef UTILITY_DEBUG_FIFO
			printf("ERROR:Rb is NULL\n");
        #endif
        return FIFO_HANDLE_NULL;
    }
    return FIFO_SUCCESS;
}

/*
*********************************************************************************************************
*	函 数 名: capacity
*	功能说明: 获取RB Size
*	形    参: 空
*	返 回 值: size
*********************************************************************************************************
*/
int32_t rbClear(rb_t* rb)
{
    int ret;

    ret = rbNotNULL(rb);
    if(ret!=0)
    {
        return ret;
    }
    
    RB_LOCK( rb );
    rb->buff = NULL;
    rb->head = NULL;
    rb->tail = NULL;
    rb->capacity = 0;
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }

    return ret;
}

/*
*********************************************************************************************************
*	函 数 名: capacity
*	功能说明: 获取RB Size
*	形    参: 空
*	返 回 值: size
*********************************************************************************************************
*/
int32_t capacity(rb_t *rb)
{
    int num,ret;

    ret = rbNotNULL(rb);
    if(ret!=0)
    {
        return ret;
    }

    RB_LOCK( rb );
    num = rb->capacity;
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }

    return num;
}

/*
*********************************************************************************************************
*	函 数 名: rbCanRead
*	功能说明: 获取可读取数据长度
*	形    参: 空
*	返 回 值: int32_t
*********************************************************************************************************
*/
int32_t rbCanRead(rb_t *rb)	
{
    int num,ret;

    /* NULL指针判断 */
    ret = rbNotNULL(rb);
    if(ret!=0)
    {
        printf("Error[%d]:%d\n",ret,__LINE__);
        return ret;
    }

    /* 上锁 */
    RB_LOCK( rb );
    if ( rb->head == rb->tail )
    {
        num = 0;
    }else if (rb->head < rb->tail)	
    {
        num = rb->tail - rb->head;
    }else{
        num = rb->capacity - (rb->head - rb->tail);
    }
    /* 解锁 */
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }
    
    return num;
}

/*
*********************************************************************************************************
*	函 数 名: rbCanWrite
*	功能说明: 获取剩余内存
*	形    参: 空
*	返 回 值: int32_t
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*	函 数 名: rbCopy
*	功能说明: 数据拷贝，但不删除
*	形    参: 空
*	返 回 值: int32_t
*********************************************************************************************************
*/
int32_t rbCopy(rb_t *rb, void *data, size_t count)
{
    int copySz = 0,ret;

    ret = rbNotNULL(rb);
    if( ret != FIFO_SUCCESS )
    {
        return ret;
    }

    if(NULL == data) {
		#ifdef UTILITY_DEBUG_FIFO
			printf("ERROR rbCopy: input data is NULL\n");
		#endif
        return FIFO_HANDLE_NULL;
    }

    RB_LOCK( rb );
    /* 头指针小于尾指针 */
    if(rb->head < rb->tail) 
    {
        copySz = min(count, rb->tail - rb->head); 
        memcpy(data, rb->head, copySz);						
    }
    else if( rb->head == rb->tail )
    {
        copySz = 0;
    }
    else
    {
        if( count < rb->capacity-(rb->head - rb->buff))  /* 读取的比数据到结尾的要少 */
        {
            copySz = count;
            memcpy(data, rb->head, copySz);
        }
        /* rb需要从尾转到头 */
        else 
        {
            copySz = rb->capacity - (rb->head - rb->buff);
            memcpy(data, rb->head, copySz);
            count = min(count-copySz,(rb->tail-rb->buff));
            memcpy((uint8_t*)data+copySz,rb->buff,count);
            copySz+=count;
        }
    }
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }
    return copySz;	
}

/*
*********************************************************************************************************
*	函 数 名: rbDelete
*	功能说明: 数据删除,数据偏移掉,但数据不置0
*	形    参: 空
*	返 回 值: 实际删除的数据
*********************************************************************************************************
*/
int32_t rbDelete(rb_t *rb,size_t count)
{
    int copySz = 0,ret;
    ret = rbNotNULL(rb);
    if(ret!=0)
    {
        return ret;
    }
    
    RB_LOCK( rb );
    if (rb->head < rb->tail) 
    {
        copySz = min( count, rb->tail - rb->head ); 	
        rb->head += copySz;
    }
    /* 不进行操作，不管是一轮了还是初始化 */
    else if(rb->head == rb->tail) 
    {

    }
    else 
    {
        if( count < rb->capacity - ( rb->head - rb->buff )) 
        {
            copySz = count;
            rb->head += copySz;
        }
        else 
        {
            copySz = rb->capacity - (rb->head - rb->buff);
            rb->head = rb->buff;
            count = min(count-copySz, rb->capacity - (rb->head - rb->tail)); 	
            rb->head += count;
            copySz += count;
        }
    }
    ret = RB_UNLOCK( rb );


    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }
    return copySz;
}

/*
*********************************************************************************************************
*	函 数 名: rbWrite
*	功能说明: 数据写入,会直接覆盖掉
*	形    参: 空
*	返 回 值: 实际写入的有效数据，若覆盖则会减去背覆盖的数量
*********************************************************************************************************
*/
int32_t rbWrite(rb_t *rb, const void *data, size_t count)
{
    int tailAvailSz = 0,ret;

    ret = rbNotNULL(rb);
    if( ret != 0 )
    {
        return ret;
    }

    if(NULL == data) {
		#ifdef UTILITY_DEBUG_FIFO
			printf("ERROR rbWrite: data is empty \n");
		#endif
        return -1;
    }

    ret = rbCanWrite(rb);
    if (count > ret ) {
		#ifdef UTILITY_DEBUG_FIFO
			printf("ERROR rbWrite: %d no memory %d \n", (int)count , ret );
		#endif
        return -2;
    }

    RB_LOCK( rb );
    if (rb->head <= rb->tail) 
    {
        /* 剩余大小 */
        tailAvailSz = rb->capacity - (rb->tail - rb->buff);
        if ( count <= tailAvailSz ) 
        {														
            memcpy(rb->tail, data, count);
            rb->tail += count;
             /* 刚好塞满 */
            if (rb->tail == rb->buff+rb->capacity) 
            {
                rb->tail = rb->buff;
            }
            ret = count;
        } 
        else 
        {													
            memcpy(rb->tail, data, tailAvailSz);
            rb->tail = rb->buff; //重置指针
            count -= tailAvailSz;   //计算剩余大小
            if( rb->tail + count >= rb->head ) //将要超越Head,也就是会覆盖数据
            {
                // rb->head += 
            }
                
            memcpy( rb->tail, (char*)data+tailAvailSz, count);
            rb->tail += count;

            ret = tailAvailSz;
        }
    }
    else 
    {
        memcpy(rb->tail, data, count);
        rb->tail += count;

        ret = count;
    }
    
    ret = RB_UNLOCK( rb );
    if( ret < 0 ){
        printf("ERROR:%d\r\n",__LINE__);
    }
    return count;
}





