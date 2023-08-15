
#include "utility_fifo.h"

#if EIOT_LIB_FIFO

/*
*********************************************************************************************************
*	函 数 名: rbCreate
*	功能说明: 创建rb
*	形    参: 空
*	返 回 值: size
*********************************************************************************************************
*/
void rbCreate(rb_t* rb,uint8_t *Buff,int32_t size)
{
    if(NULL == rb)
    {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbCreate: input rb is NULL\n");
		#endif
        return;
    }
    rb->rbBuff = Buff;
    rb->rbHead = rb->rbBuff;		
    rb->rbTail = rb->rbBuff;	
    rb->rbCapacity = size;		
    rb->mutex = NULL;
    
#if EIOT_SYSTEM_USE_OS
    rb->mutex = ( void* )libcOsRecursiveMutexNew();
    if ( rb->mutex == NULL ) {
        printf("rb->mutex is NULL\n");
        while (1) {}
    }
#endif
    
}

/*
*********************************************************************************************************
*	函 数 名: rbCapacity
*	功能说明: 获取RB Size
*	形    参: 空
*	返 回 值: size
*********************************************************************************************************
*/
void rbClear(rb_t* rb)
{
    if(NULL == rb)
    {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR: input rb is NULL\n");
		#endif
        return;
    }
    
    
    RB_LOCK( rb );
    rb->rbBuff = NULL;
    rb->rbHead = NULL;
    rb->rbTail = NULL;
    rb->rbCapacity = 0;
    RB_UNLOCK( rb );//1
}

/*
*********************************************************************************************************
*	函 数 名: rbCapacity
*	功能说明: 获取RB Size
*	形    参: 空
*	返 回 值: size
*********************************************************************************************************
*/
int32_t rbCapacity(rb_t *rb)
{
    int num;
    
    if(NULL == rb)
    {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbCapacity: input rb is NULL\n");
		#endif
        return -1;
    }

    RB_LOCK( rb );
    num = rb->rbCapacity;
    RB_UNLOCK( rb );//2
    return num;
}

/*
*********************************************************************************************************
*	函 数 名: rbCanWrite
*	功能说明: 获取可读取数据长度
*	形    参: 空
*	返 回 值: int32_t
*********************************************************************************************************
*/
int32_t rbCanRead(rb_t *rb)	
{
    int num;
    
    if(NULL == rb)
    {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbCanRead: input rb is NULL\n");
		#endif
        return -1;
    }

    RB_LOCK( rb );
    if (rb->rbHead == rb->rbTail)
    {
        RB_UNLOCK( rb );
        return 0;
    }

    if (rb->rbHead < rb->rbTail)	
    {
        num = rb->rbTail - rb->rbHead;
        RB_UNLOCK( rb );
        return num;
    }
	
    num = rbCapacity(rb) - (rb->rbHead - rb->rbTail);
    RB_UNLOCK( rb );
    
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
int32_t rbCanWrite(rb_t *rb)				
{
    int num;
    if(NULL == rb)
    {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbCanWrite: input rb is NULL\n");
		#endif
        return -1;
    }

    RB_LOCK( rb );
    num = rbCapacity(rb) - rbCanRead(rb);	
    RB_UNLOCK( rb );//4
    
    return num;
}

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
    int copySz = 0;

    if(NULL == rb) {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbCopy: input rb is NULL\n");
		#endif
        return -1;
    }

    if(NULL == data) {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbCopy: input data is NULL\n");
		#endif
        return -1;
    }

    RB_LOCK( rb );
    /* 头指针小于尾指针 */
    if (rb->rbHead < rb->rbTail) 
    {
        copySz = min(count, rbCanRead(rb)); 
        memcpy(data, rb->rbHead, copySz);	
        RB_UNLOCK( rb ); //6   
        return copySz;						
    }
    else
    {
        /* 读取的比数据到结尾的要少 */
        if( count < rbCapacity(rb)-(rb->rbHead - rb->rbBuff)) 
        {
            copySz = count;
            memcpy(data, rb->rbHead, copySz);
            RB_UNLOCK( rb );//6
            return copySz;
        }
        /* rb需要从尾转到头 */
        else 
        {
            copySz = rbCapacity(rb) - (rb->rbHead - rb->rbBuff);
            memcpy(data, rb->rbHead, copySz);
            count = min(count-copySz,(rb->rbTail-rb->rbBuff));
            memcpy((uint8_t*)data+copySz,rb->rbBuff,count);
            RB_UNLOCK( rb );//6
            return copySz+count;
        }
    }	
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
    int copySz = 0;

    if(NULL == rb)
    {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbDelete: input rb is NULL\n");
		#endif
        return -1;
    }
    
    RB_LOCK( rb );
    if (rb->rbHead < rb->rbTail) 
    {
        copySz = min(count, rbCanRead(rb)); 	
        rb->rbHead += copySz;
        RB_UNLOCK( rb );//7
        return copySz;						
    }
    /* 不可能等于0，除非初始化,当前数据写入满了会覆盖 */
    else if(rb->rbHead == rb->rbTail) 
    {
        RB_UNLOCK( rb );//7
		return 0;
    } 
    else 
    {
        if (count < rbCapacity(rb)-(rb->rbHead - rb->rbBuff)) 
        {
            copySz = count;
            rb->rbHead += copySz;
            RB_UNLOCK( rb );//7
            return copySz;
        } 
        else 
        {
            copySz = rbCapacity(rb) - (rb->rbHead - rb->rbBuff);
            rb->rbHead = rb->rbBuff;
            #if 0
            /* 递归会不会太耗时间,典型的时间换空间，没必要，因为不会存在第二次数据环重复掉头 */
            /* 存在重复上锁解锁 */
            copySz += rbDelete(rb,count-copySz);
            #else
            count = min(count-copySz, rbCanRead(rb)); 	
            rb->rbHead += count;
            copySz += count;
            #endif
            RB_UNLOCK( rb );//7
            return copySz;
        }
    }
}

/*
*********************************************************************************************************
*	函 数 名: rbWrite
*	功能说明: 数据写入,会直接覆盖掉
*	形    参: 空
*	返 回 值: 实际删除的数据
*********************************************************************************************************
*/
int32_t rbWrite(rb_t *rb, const void *data, size_t count)
{
    int tailAvailSz = 0;

    if(NULL == rb) {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbWrite: rb is empty \n");
		#endif
        return -1;
    }

    if(NULL == data) {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbWrite: data is empty \n");
		#endif
        return -1;
    }

    RB_LOCK( rb );
    if (count >= rbCanWrite(rb) - 1) {
		#ifdef EIOT_DEBUG_FIFO
			printf("ERROR rbWrite: %d no memory %d \n", count , rbCanWrite(rb));
		#endif
        RB_UNLOCK( rb );//8
        return -1;
    }

    if (rb->rbHead <= rb->rbTail) 
    {
        /* 剩余大小 */
        tailAvailSz = rbCapacity(rb) - (rb->rbTail - rb->rbBuff);
        if ( count <= tailAvailSz ) 
        {														
            memcpy(rb->rbTail, data, count);
            rb->rbTail += count;
            /* 刚好塞满 */
            if (rb->rbTail == rb->rbBuff+rbCapacity(rb)) 
            {
                rb->rbTail = rb->rbBuff;
            }
            RB_UNLOCK( rb );//8
            return count;
        } 
        else 
        {													
            memcpy(rb->rbTail, data, tailAvailSz);
            rb->rbTail = rb->rbBuff;
            #if 0
            tailAvailSz += rbWrite(rb, (char*)data+tailAvailSz, count-tailAvailSz);
            #else
            count -= tailAvailSz;
            memcpy( rb->rbTail, (char*)data+tailAvailSz, count);
            rb->rbTail += count;
            #endif

            RB_UNLOCK( rb );//8
            return tailAvailSz;
        }
    } 
    else 
    {
        memcpy(rb->rbTail, data, count);
        rb->rbTail += count;
        RB_UNLOCK( rb );//8
        return count;
    }
}








#endif
