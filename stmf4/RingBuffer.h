#ifndef 	_RingBuffer_h
#define 	_RingBuffer_h

#include <stdio.h>
#include <string.h>
#include "Uart.h"
#include "queue.h"
#include "Assert.h"

typedef struct RingBuffer {
	char *buffer;
	int head, tail, active;
	int inc, bufferSize;
} RingBuffer;

void RingBufferStructInit(RingBuffer* me);
char* RingBufferConstr(RingBuffer* me, u32 sizeBuffer, u32 sizeItem);
void PushRingBuffer(RingBuffer* me, void* item);
int PopRingBuffer(RingBuffer* me, void *item);
void *PeekRingBuffer(RingBuffer* me);
int GetNumberItemLeft(RingBuffer* me);
void FlushRingBuffer(RingBuffer* me);

#endif
