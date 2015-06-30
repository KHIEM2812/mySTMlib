#include "TRingBuffer.h"
RingBuffer myCBQQueue;

int DisplayCBQ(RingBuffer* me) {
	int i = 0, j = 0;

	tCmdStruct * currentCmd;

	if (!me->active) {
		myPrintf1("has NO cmd in Queue\r\n");
		return -1;
	}

	i = GetNumberItemLeft(me) - 1;
	myPrintf3("myQueue has %d cmd\r\n", i + 1);

	do {
		j = (me->tail + i * me->inc);
		if (j >= me->bufferSize)
			j -= me->bufferSize;

		currentCmd = (tCmdStruct *) &me->buffer[j];
		myPrintf3("my %d-Cmd: %d,%d,%d\r\n", j, currentCmd->forceSetpoint,
				currentCmd->positionSetpoint, currentCmd->velocitySetpoint);
	} while (i-- != 0);
	myPrintf1("\r\n");
	return 0;
}

void TCBCQueue() {
	char myString[100];
	RingBufferStructInit(&myCBQQueue);
	RingBufferConstr(&myCBQQueue, 2 * sizeof(tCmdStruct), sizeof(tCmdStruct));

	DisplayCBQ(&myCBQQueue);

	tCmdStruct fistCmd = { 0, 100, 200, 300 };
	myPrintf1("push 1stCmd\r\n");
	PushRingBuffer(&myCBQQueue, &fistCmd);
	DisplayCBQ(&myCBQQueue);
	tCmdStruct secondCmd = { 0, 400, 500, 600 };
	myPrintf1("push 2ndCmd\r\n");
	PushRingBuffer(&myCBQQueue, &secondCmd);
	DisplayCBQ(&myCBQQueue);

	tCmdStruct secondCopy;
	myPrintf1("pop 1stCmd. And here is what i get:");
	PopRingBuffer(&myCBQQueue, &secondCopy);
	myPrintf3("%d,%d,%d\r\n", secondCopy.forceSetpoint,
			secondCopy.positionSetpoint, secondCopy.velocitySetpoint);
	DisplayCBQ(&myCBQQueue);

	myPrintf1("push 1stCmd AGAIN\r\n");
	PushRingBuffer(&myCBQQueue, &fistCmd);
	DisplayCBQ(&myCBQQueue);

	tCmdStruct thirdCmd = { 0, 700, 800, 900 };
	myPrintf1("push 3rd, CBQ d wrap around now and it has 1st n 3rd CMD\r\n");
	PushRingBuffer(&myCBQQueue, &thirdCmd);
	DisplayCBQ(&myCBQQueue);

	tCmdStruct firstCopy;
	myPrintf1("pop 1st Cmd. And here is what i get:");
	PopRingBuffer(&myCBQQueue, &firstCopy);
	myPrintf3("%d,%d,%d\r\n", firstCopy.forceSetpoint,
			firstCopy.positionSetpoint, firstCopy.velocitySetpoint);
	DisplayCBQ(&myCBQQueue);

	myPrintf1("pop 1stCmd. And here is what i get:");
	PopRingBuffer(&myCBQQueue, &secondCopy);
	myPrintf3("%d,%d,%d\r\n", secondCopy.forceSetpoint,
			secondCopy.positionSetpoint, secondCopy.velocitySetpoint);
	DisplayCBQ(&myCBQQueue);

	myPrintf1("\r\n");
	myPrintf1("i pop 1 more which shouldnt be\r\n");
	if (PopRingBuffer(&myCBQQueue, &firstCopy) == -1) {
		myPrintf1("get nothing\r\n");
	} else {
		myPrintf1("test failed\r\n");
	}
}
