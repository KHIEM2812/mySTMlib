#include "motorLib/TMotorContorller.h"
#include "motorLib/MotorController.h"
#include "myLib/cUart.h"
#include "cCommandGetter.h"

MotorController myMotor;
MotorController* ptrMotor;
void TestMotorControllerConfig() {
	MotorControllerConfigForMAT(&myMotor);
	ptrMotor = &myMotor;
}

void DisplayMotor() {
	static unsigned int lastTime = 0;
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = abs(currentTime - lastTime);
	if (deltaTime > 1000000) {
		UartPrint("motor:%d, %.1f\r\n",
				ptrMotor->motorEncoder->accumEncoderCounts,
				ptrMotor->motorData->motorCurrentSpeed);
		lastTime = currentTime;
	}
}

void TestMCConsistentBtwnMotorPortNEncoder() {
	static int velocity = 0;
	char cmdString[10];
	memset(cmdString, 0, 10);
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

	SetModeMotor(ptrMotor, OPENED_LOOP_VELOCITY); //openloop
	SetTargetMotorSpeed(ptrMotor, (float) velocity);
	UpdateMotorControlLoop(ptrMotor);
	UpdateMotorEncoderNSpeed(ptrMotor);
	DisplayMotor();

}
//test stop, set speed, ...
void TestMCVeloFBControl() {
	static int velocity = 0;
	char cmdString[10];
	memset(cmdString, 0, 10);
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

	SetModeMotor(ptrMotor, CLOSED_LOOP_VELOCITY); //openloop
	SetTargetMotorSpeed(ptrMotor, velocity);
	UpdateMotorControlLoop(ptrMotor);
	UpdateMotorEncoderNSpeed(ptrMotor);
	DisplayMotor();
}
void TestMCPosFBControl() {
	static int position = 0;
	char cmdString[10];
	memset(cmdString, 0, 10);
	char ch = ReadUart();
	CommandGetter(ch, cmdString);
	if (cmdString[0] != '\0') {
		UartPrint("get cmd: %s\r\n", cmdString);
		switch (cmdString[0]) {
		case 'p':
			position = atoi(&cmdString[1]);
			UartPrint("setPos: %d\r\n", position);
			break;
		}

	}

	SetModeMotor(ptrMotor, CLOSED_LOOP_POSITION); //openloop
	SetTargetMotorPosition(ptrMotor, position);
	UpdateMotorControlLoop(ptrMotor);
	UpdateMotorEncoderNSpeed(ptrMotor);
	DisplayMotor();
}
void TestMCMixFBControl() {
	static int position = 0;
	static int velocity = 0;
	char cmdString[10];
	memset(cmdString, 0, 10);
	char ch = ReadUart();
	CommandGetter(ch, cmdString);
	if (cmdString[0] != '\0') {
		UartPrint("get cmd: %s\r\n", cmdString);
		switch (cmdString[0]) {
		case 'p':
			position = atoi(&cmdString[1]);
			SetModeMotor(ptrMotor, CLOSED_LOOP_POSITION); //openloop
			SetTargetMotorPosition(ptrMotor, position);
			ptrMotor->motorPositionPID->oMax = 3;
			ptrMotor->motorPositionPID->pMax = 3;
			ptrMotor->motorPositionPID->dMax = 3;
			ptrMotor->motorPositionPID->iMax = 0.5;
			ptrMotor->motorVelocityPID->pGain = 0.5;
			ptrMotor->motorData->motorAccelerationLimit = 20;
			velocity = 0;
			UartPrint("setPos: %d\r\n", position);
			break;
		case 's':
			velocity = atoi(&cmdString[1]);
			SetModeMotor(ptrMotor, CLOSED_LOOP_VELOCITY); //openloop
			SetTargetMotorSpeed(ptrMotor, velocity);
			ptrMotor->motorData->motorAccelerationLimit = 5;
			ptrMotor->motorVelocityPID->pGain = 0.8;
			position = 0;
			UartPrint("setVelo: %d\r\n", velocity);
			break;
		}

	}

	UpdateMotorControlLoop(ptrMotor);
	UpdateMotorEncoderNSpeed(ptrMotor);
	DisplayMotor();
}
