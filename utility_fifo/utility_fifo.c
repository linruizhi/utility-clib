
#include "utility_fifo.h"

#if EIOT_LIB_FIFO

/*
*********************************************************************************************************
*	�� �� ��: rbCreate
*	����˵��: ����rb
*	��    ��: ��
*	�� �� ֵ: size
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
*	�� �� ��: rbCapacity
*	����˵��: ��ȡRB Size
*	��    ��: ��
*	�� �� ֵ: size
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
*	�� �� ��: rbCapacity
*	����˵��: ��ȡRB Size
*	��    ��: ��
*	�� �� ֵ: size
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
*	�� �� ��: rbCanWrite
*	����˵��: ��ȡ�ɶ�ȡ���ݳ���
*	��    ��: ��
*	�� �� ֵ: int32_t
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
*	�� �� ��: rbCanWrite
*	����˵��: ��ȡʣ���ڴ�
*	��    ��: ��
*	�� �� ֵ: int32_t
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
*	�� �� ��: rbCopy
*	����˵��: ���ݿ���������ɾ��
*	��    ��: ��
*	�� �� ֵ: int32_t
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
    /* ͷָ��С��βָ�� */
    if (rb->rbHead < rb->rbTail) 
    {
        copySz = min(count, rbCanRead(rb)); 
        memcpy(data, rb->rbHead, copySz);	
        RB_UNLOCK( rb ); //6   
        return copySz;						
    }
    else
    {
        /* ��ȡ�ı����ݵ���β��Ҫ�� */
        if( count < rbCapacity(rb)-(rb->rbHead - rb->rbBuff)) 
        {
            copySz = count;
            memcpy(data, rb->rbHead, copySz);
            RB_UNLOCK( rb );//6
            return copySz;
        }
        /* rb��Ҫ��βת��ͷ */
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
*	�� �� ��: rbDelete
*	����˵��: ����ɾ��,����ƫ�Ƶ�,�����ݲ���0
*	��    ��: ��
*	�� �� ֵ: ʵ��ɾ��������
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
    /* �����ܵ���0�����ǳ�ʼ��,��ǰ����д�����˻Ḳ�� */
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
            /* �ݹ�᲻��̫��ʱ��,���͵�ʱ�任�ռ䣬û��Ҫ����Ϊ������ڵڶ������ݻ��ظ���ͷ */
            /* �����ظ��������� */
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
*	�� �� ��: rbWrite
*	����˵��: ����д��,��ֱ�Ӹ��ǵ�
*	��    ��: ��
*	�� �� ֵ: ʵ��ɾ��������
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
        /* ʣ���С */
        tailAvailSz = rbCapacity(rb) - (rb->rbTail - rb->rbBuff);
        if ( count <= tailAvailSz ) 
        {														
            memcpy(rb->rbTail, data, count);
            rb->rbTail += count;
            /* �պ����� */
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
