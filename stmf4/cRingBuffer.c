#include "cRingBuffer.h"

void RingBufferStructInit(RingBuffer* me) {
	memset(me, 0, sizeof(RingBuffer));
	me->head = 0;
	me->tail = 0;
}

void RingBufferConfig(RingBuffer* me, uint32_t numberItems, uint32_t sizeItem) {
	me->itemSize = sizeItem;
	me->bufferSize = numberItems * sizeItem;
	me->buffer = malloc((size_t) numberItems * sizeItem);
	if (me->buffer == 0) {
		me->itemSize = 0;
		me->bufferSize = 0;
	}
}

void PushRingBuffer(RingBuffer* me, void* item) {
	memcpy((void*) &me->buffer[me->head], (void*) item, me->itemSize);

	me->head += me->itemSize;
	if (me->head >= me->bufferSize) {
		me->head -= me->bufferSize;
	}

	if (me->active < me->bufferSize) {
		me->active += me->itemSize;
	} else {
		me->tail += me->itemSize;
		if (me->tail >= me->bufferSize) {
			me->tail -= me->bufferSize;
		}
	}
}

int PopRingBuffer(RingBuffer* me, void *item) {
	if (!me->active) {
		return -1;
	}

	memcpy((void*) item, (void*) &me->buffer[me->tail], me->itemSize);

	me->tail += me->itemSize;
	if (me->tail >= me->bufferSize) {
		me->tail -= me->bufferSize;
	}
	me->active -= me->itemSize;
	return 0;
}

void * GetFirstItemPtr(RingBuffer* me) {
	return (void*) ((uint32_t) me->buffer + (uint32_t) me->tail);
}
void * GetNextItemPtr(RingBuffer* me, void* currentPtr) {
	//check if current ptr is valid
	//currentPtr must be within range
//	REQUIRE(
//			((uint32_t) me->buffer <= (uint32_t) currentPtr)
//					&& ((uint32_t) me->buffer + (uint32_t) me->bufferSize
//							>= (uint32_t) currentPtr));
	//currentPtr must be a multiple of me->itemSize
	int byteDistance = (int) ((uint32_t) currentPtr - (uint32_t) me->buffer);
//	REQUIRE((byteDistance % me->itemSize) == 0);
	(void)byteDistance;

	void *tempPtr = (void *) ((uint32_t) currentPtr + (uint32_t) me->itemSize);
	if ((uint32_t) tempPtr >= ((uint32_t) me->buffer + (uint32_t) me->bufferSize)) {
		tempPtr -= me->bufferSize;
	}

	if (((uint32_t) tempPtr - (uint32_t) me->buffer) == ((uint32_t) me->head)) {
		tempPtr = 0;
	}

	return tempPtr;
}

int GetNumberItemLeftOfRBuffer(RingBuffer* me) {
	int ret = (me->bufferSize - me->active) / me->itemSize;
	return ret;
}
int GetNumberItemUsedOfRBuffer(RingBuffer* me) {
	int ret = me->active / me->itemSize;
	return ret;
}
int GetNumberByteLeftOfRBuffer(RingBuffer* me) {
	return me->bufferSize - me->active;
}
int GetNumberByteUsedOfRBuffer(RingBuffer* me) {
	return me->active;
}
void FlushRingBuffer(RingBuffer* me) {
	me->head = 0;
	me->tail = 0;
	me->active = 0;
}
void FreeRingBuffer(RingBuffer* me) {
	if (me->itemSize != 0) {
		free(me->buffer);
		RingBufferStructInit(me);
	}
}
