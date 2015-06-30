#ifndef _cIndexInterrupt_h
#define _cIndexInterrupt_h

typedef struct IndexInterrupt {
	int id;
	unsigned int timeWhenHitIndex;
	unsigned int timeOut;
	int hitFlag; //0-not hit or hit some time ago (defined by timeOut); 1=hit index recently
} IndexInterrupt;

void IndexInterruptStructInit(IndexInterrupt * me);
void IndexInterruptConfig0(IndexInterrupt * me);
IndexInterrupt* GetIndexInterruptPtr0();
int ReadIndexFlag(IndexInterrupt * me);

#endif
