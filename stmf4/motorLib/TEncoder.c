#include "TEncoder.h"

encoder_t encoder0, encoder1;
MotorPort tMotorPors;

void TEncoderConfig() {
	Enc0Config(&encoder0);
	EnableEncoder(&encoder0);
	Enc1Config(&encoder1);
	EnableEncoder(&encoder1);
}

void DisplayEnc0Counts() {
	static unsigned int lastTime = 0;
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = abs(currentTime - lastTime);
	if (deltaTime > 1000000) {
		UartPrint("enc0:%d, %d\r\n", (int) encoder0.accumEncoderCounts,
				(int) encoder0.deltaEncoderCount);
		lastTime = currentTime;
	}
	GetEncoderCounts(&encoder0);

}
void DisplayEnc1Counts() {
	static unsigned int lastTime = 0;
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = abs(currentTime - lastTime);
	if (deltaTime > 1000000) {
		UartPrint("enc1:%d, %d\r\n", (int) encoder1.accumEncoderCounts,
				(int) encoder1.deltaEncoderCount);
		lastTime = currentTime;
	}
	GetEncoderCounts(&encoder1);
}
void TEncoder0Features() {
	char cmd = ReadUart();
	switch (cmd) {
	case 'e':
		EnableEncoder(&encoder0);
		break;
	case 'd':
		DisableEncoder(&encoder0);
		break;
	case 'r':
		ResetEncoder(&encoder0);
		break;
	case 's':
		SetEncoder(&encoder0,1000);
		break;
	}
	DisplayEnc0Counts();
}

//when motor rotate CW, encoder ++
//but when motor rotates CCW, encoder --
//supposing motor move with const speed
void TEnc0BehaviorConfig() {
	SystemTimerInit(1000000);
	Enc0Config(&encoder0);
}

enum TEnc0BehaviorSignals {
	TICK_SIG = _USER_SIG,
};
enum TEnc0BehaviorStates { /* all states for the Bomb FSM */
	START, CWSTATE, CCWSTATE, ENDTEST
};

typedef struct {
	uint8_t state;
} Enc0Behavior;
//state: START=0, CW, CCW
void TEnc0Behavior() {
	static u32 encoderC0, encoderC1, encoderC2;
	static Enc0Behavior tEnc0Behavior; //contain FSM
	static TickEvt tickEvt;
	tickEvt.super.sig = TICK_SIG;

	Event *e = (Event *) 0;
	static int firstDelta = 0, secondDelta = 0, sign = 0;

	static unsigned long lastTempTimer = 0;
	unsigned long tempTimer = GetSystemTimer();
	unsigned long tempTimer1 = tempTimer - lastTempTimer;
	if (tempTimer1 == 0) {
		e = (Event *) &tickEvt;
		lastTempTimer = tempTimer;
	}

	switch (tEnc0Behavior.state) {
	case START: //START
		tEnc0Behavior.state = CWSTATE;
		encoderC0 = GetEncoderCounts(&encoder0);
		break;
	case CWSTATE: //CW, //rotate CW for 5 sec
		SetRotate(&tMotorPors, 1000);

		if (e->sig == TICK_SIG) {
			tEnc0Behavior.state = CCWSTATE;
			encoderC1 = GetEncoderCounts(&encoder0);
		}
		break;
	case CCWSTATE: //rotate CCW for 5 sec
		SetRotate(&tMotorPors, -1000);
		if (e->sig == TICK_SIG) {
			encoderC2 = GetEncoderCounts(&encoder0);
		}
		break;
	case ENDTEST:
		firstDelta = (int) (encoderC1 - encoderC0);
		secondDelta = (int) (encoderC2 - encoderC1);
		sign = firstDelta * secondDelta;
		ENSURE(firstDelta>0);
		ENSURE(secondDelta>0);

		if (sign < 0) {
			if (firstDelta > 0 && secondDelta < 0) {
				UartPrint("OKIE.Speed>0--> enc++\r\n", 0);
			} else if (firstDelta < 0 && secondDelta > 0) {
				UartPrint("OKIE.Speed>0--> enc++\r\n", 0);
			}
		} else {
			UartPrint("FAILED!!!\r\n", 0);
		}
	}

}
