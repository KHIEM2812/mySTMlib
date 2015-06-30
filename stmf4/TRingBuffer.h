#ifndef _TRingBuffer_h
#define _TRingBuffer_h

#include "RingBuffer.h"
#include "JointControllerFSM.h"
#include "Uart.h"

typedef struct {
	uint typeCmd; //0=forceCmd, 1= posCmd, 2=veloCmd
	int positionSetpoint;
	int velocitySetpoint;
	int forceSetpoint;
} tCmdStruct;

void TCBCQueue();

#endif
