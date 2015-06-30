#ifndef _staticQueue_h
#define _staticQueue_h

#define STATICQUEUE_SIZE	2

#include <stdio.h>
#include <string.h>
#include "Uart.h"
#include "queue.h"
#include "Assert.h"

typedef struct StaticQueue {
	char buffer[STATICQUEUE_SIZE * 12];
	int head, tail, active;
	int inc;
} StaticQueue;

void QueueStructInit(StaticQueue* me);
void QueueConfig(StaticQueue* me);
void PushSQ(StaticQueue* me, NodeFCmd *item); // add item to tail
// get item from head and remove it from queue
int PopSQ(StaticQueue* me, NodeFCmd *item);
int GetNumberCmdLeft(StaticQueue* me);


#endif
