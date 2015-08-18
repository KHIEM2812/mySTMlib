#include "TestMotorPortMazherBoardInterface.h"
#include "cAlienCatherPeripheral.h"
#include "myLib/systemTimer.h"
#include "myLib/cUart.h"
#include "myLib/cCommandGetter.h"
#include "motorLib/MotorPortMazherBoardInterface.h"
#include "myLib/Assert.h"

MotorController* motor1, *motor2, *motor;

void TMotorConttrollerConfigMazherBoardInterface() {
	motor1 = malloc(sizeof(MotorController));
	motor = motor1;
	motor->id = 0;
	motor->motorMode = CLOSED_LOOP_VELOCITY;
	MotorOneControllerConfigMazherBoardInterface(motor);

	motor2 = malloc(sizeof(MotorController));
	motor = motor2;
	motor->id = 1;
	motor->motorMode = CLOSED_LOOP_VELOCITY;
	MotorTwoControllerConfigMazherBoardInterface(motor);
}
/*
 * test case:
 * 	-->disable motor--> send movePWM--> clear pwm
 * 	-->enable motor--> send movePWM --> stop Motor-> sendPWM--> disable motor
 */
void DisplayEncoderInfoInTMotorPortMazherBoardInterface() {
	static unsigned int lastTime = 0;
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = currentTime - lastTime;
	if (deltaTime > 10000) {
		UartPrint("m1(p:%d,v:%d)\r\n", motor1->motorData->motorCurrentPosition,
				(int) motor1->motorData->motorCurrentSpeed);
		UartPrint("m2(p:%d,v:%d)\r\n", motor2->motorData->motorCurrentPosition,
				(int) motor2->motorData->motorCurrentSpeed);
		lastTime = currentTime;
	}
}
void UpdateMotorInTMotorPortMazherBoardInterface() { //to see if encoder working
	UpdateMotorEncoderNSpeed(motor1);
	UpdateMotorEncoderNSpeed(motor2);
}
void TMotorPortMazherBoardInterface() {
	motor = motor2;
	static int pwm = 0;
	char cmdString[10];
	memset(cmdString, 0, 10);
	char ch = ReadUart();
	CommandGetter(ch, cmdString);
	if (cmdString[0] != '\0') {
		UartPrint("get cmd: %s\r\n", cmdString);
		switch (cmdString[0]) {
		case 'm':
			pwm = atoi(&cmdString[1]);
			UartPrint("setVelo: %d\r\n", pwm);
			SetRotate(motor->motorPort, pwm);
			break;
		case 'e': //enable
			EnableMotor(motor->motorPort);
			break;
		case 'd': //disable
			DisableMotor(motor->motorPort);
			break;
		}

	}
	//print debug msg here
	UpdateMotorInTMotorPortMazherBoardInterface();
	DisplayEncoderInfoInTMotorPortMazherBoardInterface();
}

void UpdateMotorInTMotorControllerMazherBoardInterface() { //to see if encoder working
	UpdateMotorEncoderNSpeed(motor1);
	UpdateMotorEncoderNSpeed(motor2);
	UpdateMotorControlLoop(motor1);
	UpdateMotorControlLoop(motor2);
}

void DisplayMotorData1(MotorController * me) {
	static unsigned int lastTime = 0;
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = abs(currentTime - lastTime);
	if (deltaTime > 10000) {
		DEBUG(LOG_INFO, "(p:%d,ps:%d,pwm:%d,v:%.1f\r\n",
				(int) me->motorData->motorCurrentPosition, (int) me->motorData->motorPositionSetpoint, (int) me->motorData->motorOutputPWMWithLimit, me->motorData->motorCurrentSpeed);
		lastTime = currentTime;
	}
}

void TMotorControllerMazherBoardInterface() {
	motor = motor2;
	int velo;
	int pos;
	int mode;
	char cmdString[10];
	memset(cmdString, 0, 10);
	char ch = ReadUart();
	CommandGetter(ch, cmdString);
	if (cmdString[0] != '\0') {
		UartPrint("get cmd: %s\r\n", cmdString);
		switch (cmdString[0]) {
		case 'v':
			velo = atoi(&cmdString[1]);
			UartPrint("setVelo: %d\r\n", velo);
			motor->motorMode = CLOSED_LOOP_VELOCITY;
			SetTargetMotorVelocity(motor, velo);
			break;
		case 'p':
			pos = atoi(&cmdString[1]);
			UartPrint("setPos: %d\r\n", pos);
			motor->motorMode = CLOSED_LOOP_POSITION;
			SetTargetMotorPosition(motor, pos);
			break;
		case 'e': //enable
			UartPrint("enable motor\r\n");
			EnableMotor(motor->motorPort);
			break;
		case 'd': //disable
			UartPrint("disnable motor\r\n");
			DisableMotor(motor->motorPort);
			break;
		case 'm':
			mode = atoi(&cmdString[1]);
			SetModeMotor(motor, mode);
			break;
		}

	}
	//print debug msg here
	UpdateMotorInTMotorControllerMazherBoardInterface();
	DisplayMotorData1(motor);
//	DisplayMotorData1(motor1);
}
void TSetStopMazherBoardInterface();
void TSetSoftStopMazherBoardInterface();
void TEnableMotorMazherBoardInterface();
void TDisableMotorMazherBoardInterface();
