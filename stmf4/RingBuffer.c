#include "RingBuffer.h"

void RingBufferStructInit(RingBuffer* me) {
	me->head = 0;
	me->tail = 0;
	me->active = 0;
	me->inc = 0;
	me->bufferSize = 0;
	me->buffer = 0;
}

char* RingBufferConstr(RingBuffer* me, u32 sizeBuffer, u32 sizeItem) {
	me->buffer = malloc((size_t) sizeBuffer);
	if (me->buffer != 0) {
		me->inc = sizeItem;
		me->bufferSize = sizeBuffer;
	}
	return me->buffer;
}

void PushRingBuffer(RingBuffer* me, void* item) {
	memcpy((void*) &me->buffer[me->head], (void*) item, me->inc);

	me->head += me->inc;
	if (me->head >= me->bufferSize) {
		me->head -= me->bufferSize;
	}

	if (me->active < me->bufferSize) {
		me->active += me->inc;
	} else {
		me->tail += me->inc;
		if (me->tail >= me->bufferSize) {
			me->tail -= me->bufferSize;
		}
	}
}

int PopRingBuffer(RingBuffer* me, void *item) {
	if (!me->active) {
		return -1;
	}

	memcpy((void*) item, (void*) &me->buffer[me->tail], me->inc);

	me->tail += me->inc;
	if (me->tail >= me->bufferSize) {
		me->tail -= me->bufferSize;
	}
	me->active -= me->inc;
	return 0;
}

void *PeekRingBuffer(RingBuffer* me) {
	return &me->buffer[me->head - me->inc];
}

int GetNumberItemLeft(RingBuffer* me) {
	int ret = me->active / me->inc;
	return ret;
}

void FlushRingBuffer(RingBuffer* me) {
	me->head = 0;
	me->tail = 0;
	me->active = 0;
}
