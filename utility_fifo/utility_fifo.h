

#ifndef _EIOT_FIRST_IN_FIRST_OUT_H
#define _EIOT_FIRST_IN_FIRST_OUT_H

#include "libarch.h"

#if EIOT_LIB_FIFO

#ifndef EIOT_DEBUG_FIFO
#define EIOT_DEBUG_FIFO     1
#endif

#define min(a, b) ((a)<(b)?(a):(b))                 

#if EIOT_SYSTEM_USE_OS
#define RB_LOCK( rb )       libcOsRecursiveMutexLock( rb->mutex )
#define RB_UNLOCK( rb )     libcOsRecursiveMutexUnlock( rb->mutex )

#else
#define RB_LOCK( rb )    
#define RB_UNLOCK( rb )   
#endif

typedef struct {
    size_t   rbCapacity;
    uint8_t  *rbHead;
    uint8_t  *rbTail;
    uint8_t  *rbBuff;

#if EIOT_SYSTEM_USE_OS
    void* mutex;
#endif    
    
}rb_t;

void rbCreate(rb_t* rb,uint8_t *Buff,int32_t size);
void rbClear(rb_t* rb);
int32_t rbCapacity(rb_t *rb);
int32_t rbCanRead(rb_t *rb)	;
int32_t rbCanWrite(rb_t *rb);
int32_t rbRead(rb_t *rb, void *data, size_t count);
int32_t rbCopy(rb_t *rb, void *data, size_t count);
int32_t rbDelete(rb_t *rb,size_t count);
int32_t rbWrite(rb_t *rb, const void *data, size_t count);

#endif

#endif

