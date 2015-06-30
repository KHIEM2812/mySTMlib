#include "TraceBuffer.h"

static TraceBuffer myTraceBuffer;

//must be put at the end of the config function
TraceBuffer* GetTraceBufferPtr() {
	TraceBuffer* me = &myTraceBuffer;
	if (me->thisTraceBuffPtr == 0) {
		me->thisTraceBuffPtr = me;
		me->traceBuffer = malloc(sizeof(RingBuffer));
		REQUIRE(me->traceBuffer!=0);
		RingBufferStructInit(me->traceBuffer);
		REQUIRE(
				RingBufferConstr(me->traceBuffer, TRACEBUFF_SIZE, sizeof(char)) != 0);
	}
	return me->thisTraceBuffPtr;
}

int SendTrace() {
	TraceBuffer* me = GetTraceBufferPtr();
	char tempChar = 0;
	REQUIRE(me!=0);
	while (GetNumberItemLeft(me->traceBuffer)) {
		REQUIRE(PopRingBuffer(me->traceBuffer, &tempChar)!=-1);
		myPutChar(tempChar);
	}
	return 0;
}

inline void PushBytesIntoTrace(const char * trace, size_t size) {
	TraceBuffer* me = GetTraceBufferPtr();
	u8 temp = 0;
	u32 i = 0;
	i = (u32) (size - 1);
	for (i = 0; i < size; i++) {
		temp = (*trace >> (8 * i)) & 0xff;
		PushRingBuffer(me->traceBuffer, &temp);
	}
}

void Push1ByteIntoTrace(const u8 trace) {
	const char * temp = (char*) &trace;
	PushBytesIntoTrace(temp, 1);
}

void Push2ByteIntoTrace(const u16 trace) {
	char * temp = (char*) &trace;
	PushBytesIntoTrace(temp, 2);
}

void Push4ByteIntoTrace(u32 trace) {
	char * temp = (char*) &trace;
	PushBytesIntoTrace(temp, 4);
}

void PushTraceTabIntoTrace() {
	TraceBuffer* me = GetTraceBufferPtr();
	u8 temp = 0xff;
	PushRingBuffer(me->traceBuffer, &temp);
	PushRingBuffer(me->traceBuffer, &temp);
	PushRingBuffer(me->traceBuffer, &temp);
	PushRingBuffer(me->traceBuffer, &temp);
}
void PushTraceEndIntoTrace() {
	TraceBuffer* me = GetTraceBufferPtr();
	u8 temp1 = 0x0a, temp2 = 0x0d;
	PushRingBuffer(me->traceBuffer, &temp1);
	PushRingBuffer(me->traceBuffer, &temp2);
	PushRingBuffer(me->traceBuffer, &temp1);
	PushRingBuffer(me->traceBuffer, &temp2);
}

void RegisterIntoList(const char* funcStr, unsigned int funcAdd) {
	FuncInfo * me = (FuncInfo*) malloc(sizeof(FuncInfo));
	REQUIRE(me!=0);
	PushRingBuffer(&functionList, me);
	free(me);
	me = (FuncInfo*) PeekRingBuffer(&functionList);

	me->name = (char*) malloc(strlen(funcStr) + 1);
	strcpy(me->name, funcStr);
	me->fn = funcAdd;
}
int SendList() {
	TraceBuffer* me = GetTraceBufferPtr();
	FuncInfo tempFI;
	REQUIRE(me!=0);
	while (GetNumberItemLeft(&functionList)) {
		REQUIRE(PopRingBuffer(&functionList, &tempFI)!=-1);
		myPrintf3("%s-%X\r\n", tempFI.name, tempFI.fn);
	}
	return 0;
}
