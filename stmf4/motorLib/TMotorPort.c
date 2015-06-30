#include "motorLib/TMotorPort.h"
#include "cCommandGetter.h"

MotorPort myMotorPort;
int opt;
void TMotorPortConfig() {
	MotorPort1Config(&myMotorPort);
}


void TSetRotate() {
	static int velocity = 0;
	char cmdString[10];
	memset(cmdString,0,10);
	char ch = ReadUart();
	CommandGetter(ch, cmdString);
	if (cmdString[0] != '\0') {
		UartPrint("get cmd: %s\r\n", cmdString);
		switch (cmdString[0]) {
		case 's':
			velocity = atoi(&cmdString[1]);
			UartPrint("setVelo: %d\r\n", velocity);
			break;
		}

	}

	SetRotate(&myMotorPort, velocity);
	//print debug msg here
}
