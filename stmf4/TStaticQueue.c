#include "TStaticQueue.h"

StaticQueue myStaticQueue;

int Display(StaticQueue* me) {
	char myString[100];
	int i = 0;
	int myHead = me->head / sizeof(NodeFCmd);
	NodeFCmd * currentCmd;

	if (!me->active) {
		myPrintf1("has NO cmd in Queue\r\n");
		return -1;
	}

	sprintf(myString, "myQueue has %d cmd\r\n", GetNumberCmdLeft(me));
	myPrintf1(myString);
	i = me->active / sizeof(NodeFCmd) - 1;
	do {
		currentCmd = (NodeFCmd *) &me->buffer[me->head
				- (i + 1) * sizeof(NodeFCmd)];
		sprintf(myString, "my %d-Cmd: %d,%d,%d\r\n", myHead,
				currentCmd->forceSetpoint, currentCmd->positionSetpoint,
				currentCmd->velocitySetpoint);
		myPrintf1(myString);
	} while (i-- != 0);
	myPrintf1("\r\n");
	return 0;
}

void TStaticQueue() {
	char myString[100];
	QueueStructInit(&myStaticQueue);
	QueueConfig(&myStaticQueue);

	Display(&myStaticQueue);

	NodeFCmd fistCmd = { 0.1, 0.1, 0.1 };
	myPrintf1("push 1stCmd\r\n");
	PushSQ(&myStaticQueue, &fistCmd);
	Display(&myStaticQueue);
	NodeFCmd secondCmd = { 2.0, 3.0, 4.0 };
	myPrintf1("push 2ndCmd\r\n");
	PushSQ(&myStaticQueue, &secondCmd);
	Display(&myStaticQueue);

	NodeFCmd secondCopy;
	myPrintf1("pop 2ndCmd. And here is what i get:");
	PopSQ(&myStaticQueue, &secondCopy);
	sprintf(myString, "%d,%d,%d\r\n", secondCopy.forceSetpoint,
			secondCopy.positionSetpoint, secondCopy.velocitySetpoint);
	myPrintf1(myString);
	Display(&myStaticQueue);

	NodeFCmd firstCopy;
	myPrintf1("pop 1stCmd. And here is what i get:");
	PopSQ(&myStaticQueue, &firstCopy);
	sprintf(myString, "%d,%d,%d\r\n", firstCopy.forceSetpoint,
			firstCopy.positionSetpoint, firstCopy.velocitySetpoint);
	myPrintf1(myString);
	Display(&myStaticQueue);

	myPrintf1("\r\n");
	myPrintf1("i pop 1 more which shouldnt be\r\n");
	if (PopSQ(&myStaticQueue, &firstCopy) == -1) {
		myPrintf1("get nothing\r\n");
	} else {
		myPrintf1("test failed\r\n");
	}

}
