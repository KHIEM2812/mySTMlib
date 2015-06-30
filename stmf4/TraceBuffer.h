#ifndef 	_TraceBuffer_h
#define 	_TraceBuffer_h

#ifdef _TraceBuffer_h
#define EXT
#else
#define EXT extern
#endif

#define TRACE_ENABLE		1
#define TRACEBUFF_SIZE 		256
#define MAXFUNCLIST			50

#include "systemTimer.h"
#include "Uart.h"
#include "Assert.h"
#include "RingBuffer.h"

typedef struct _tag_funcinfo {
	unsigned int fn;
	char *name;
} FuncInfo;

typedef struct TraceBuffer {
	struct TraceBuffer * thisTraceBuffPtr;
	RingBuffer* traceBuffer;
} TraceBuffer;

typedef enum TraceRecords {
	_STATE_ENTRY,
	_STATE_EXIT,
	_STATE_INIT,
	_STATE_TRANS,
	_STATE_IGNORED,

	_MUTEX_LOCK,
	_MUTEX_UNLOCK,

	_INTERRUPT_ENTRY,
	_INTERRUPT_EXIT,
	_INTERRUPT_DISABLE,
	_INTERRUPT_ENABLE,
	_CRIT_ENTRY,
	_CRIT_EXIT,

	_BUFFER_REPORT,
	_BUFFER_OVERFLOWED,

	_GLOBALVAR_CHANGED,
	_TEST_FAILED,
	_ASSERT_FAILED,
	_USERS_RECORDS

} TraceRecords;

EXT RingBuffer functionList;

#define TRACE_INIT()								GetTraceBufferPtr()
#define FUNCLIST_INIT()								RingBufferStructInit(&functionList);\
		REQUIRE(RingBufferConstr(&functionList, MAXFUNCLIST* sizeof(FuncInfo), sizeof(FuncInfo)) != 0)

#define TRACE_BEGIN(_record)						Push2ByteIntoTrace((_record<<8)+0x23)
#define TRACE_TIMESTAMP()							Push4ByteIntoTrace((u32)GetSystemTimer())
#define TRACE_STATE()								Push4ByteIntoTrace(((u32)((FSM*)me)->state))
#define TRACE_SIGNAL()								Push1ByteIntoTrace((u8)e->sig)
#define TRACE_INTERRUPT(_interrupt)					Push4ByteIntoTrace((u32)_interrupt)
#define TRACE_BUFFERADDR(_buffAdd)					Push4ByteIntoTrace((u32)_buffAdd)
#define TRACE_BUFFLEFT(_buffleft)					Push1ByteIntoTrace((u8)_buffleft)

#define REGISTER_STATE(_stateAdd)					Push4ByteIntoTrace((u32)_stateAdd)
#define REGISTER(_Str, _Add)						RegisterFunction(_Str,((u32)_Add))

#define TRACE_TAB()									PushTraceTabIntoTrace()
#define TRACE_END()									PushTraceEndIntoTrace()

#define SEND_TRACE()								SendTrace()
#define SEND_LIST()									SendList()

TraceBuffer* GetTraceBufferPtr();
int SendTrace();
int SendList();
void Push1ByteIntoTrace(u8 trace);
void Push2ByteIntoTrace(u16 trace);
void Push4ByteIntoTrace(u32 trace);
void PushTraceTabIntoTrace();
void PushTraceEndIntoTrace();
void RegisterIntoList(const char* funcStr, unsigned int funcAdd);

void FindFuncName(u32 funcAdd);
void PrintStackTrace();
#undef EXT
#endif
