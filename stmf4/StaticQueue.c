#include "staticQueue.h"

void QueueConfig(StaticQueue* me) {
	me->inc = sizeof(NodeFCmd);
}

void PushSQ(StaticQueue* me, NodeFCmd *item) {
	memcpy((void*) &me->buffer[me->head], (void*) item, me->inc);

	me->head += me->inc;
	me->active += me->inc;
	REQUIRE(me->head<STATICQUEUE_SIZE);
	REQUIRE(me->active<STATICQUEUE_SIZE);
}

int PopSQ(StaticQueue* me, NodeFCmd *item) {
	if (!me->active) {
		return -1;
	}

	memcpy((void*) item, (void*) &me->buffer[me->tail], me->inc);

	me->tail += me->inc;
	me->active -= me->inc;

	REQUIRE(me->tail<STATICQUEUE_SIZE);
	REQUIRE(me->active>=STATICQUEUE_SIZE);
	return 0;
}

int GetNumberCmdLeft(StaticQueue* me) {
	int ret = me->active / me->inc;
	return ret;
}

void QueueStructInit(StaticQueue* me) {
	me->head = 0;
	me->tail = 0;
	me->inc = 0;
	me->active = 0;
}
