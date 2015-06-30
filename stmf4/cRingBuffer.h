#ifndef 	_cRingBuffer_h
#define 	_cRingBuffer_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
//#include "myLib/Assert.h"

typedef struct RingBuffer {
	char *buffer;
	int head, tail, active;
	int itemSize, bufferSize;
} RingBuffer;

void RingBufferStructInit(RingBuffer* me);
void RingBufferConfig(RingBuffer* me, uint32_t sizeBuffer, uint32_t sizeItem);
void PushRingBuffer(RingBuffer* me, void* item);
int PopRingBuffer(RingBuffer* me, void *item);
int GetNumberItemLeftOfRBuffer(RingBuffer* me);
int GetNumberItemUsedOfRBuffer(RingBuffer* me);
int GetNumberByteLeftOfRBuffer(RingBuffer* me);
int GetNumberByteUsedOfRBuffer(RingBuffer* me);

//this routine faster than GetNumberItemLeftOfRBuffer in checking if the buffer avail
int GetNumberByteLeftOfRBuffer(RingBuffer* me);
void * GetFirstItemPtr(RingBuffer* me);
void * GetNextItemPtr(RingBuffer* me, void* currentPtr);

void FlushRingBuffer(RingBuffer* me);
void FreeRingBuffer(RingBuffer* me);
#endif
