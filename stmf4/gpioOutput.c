#include "gpioOutput.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "myLib/systemTimer.h"

void OutputPinInitStruct(OutputPinStruct *me, int type) {
	if (type == 0) {
		memset(me, 0, sizeof(OutputPinStruct));
	} else if (type == 1) {
		memset(me, 0, sizeof(PatternedOutputPinStruct));
	}
}
void OffOutputPin(OutputPinStruct *me) {
	GPIO_WriteBit(me->myGPIO, me->GPIOPin, 0);
}
void OnOutputPin(OutputPinStruct *me) {
	GPIO_WriteBit(me->myGPIO, me->GPIOPin, 1);
}
void SetPatternOutputPin(PatternedOutputPinStruct *me, int timeOn, int timeOff) {
	if (timeOn == -1 && timeOff == 0) {
		OnOutputPin((OutputPinStruct*) me);
	} else if (timeOn == 0 && timeOff == -1) {
		OffOutputPin((OutputPinStruct*) me);
	}
	me->timeToOff = timeOff;
	me->timeToOn = timeOn;
}
void OutputPinPatternRun(PatternedOutputPinStruct *me) {
	if (me->timeToOff < 0 || me->timeToOn < 0) {
		return;
	}
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = abs(currentTime - me->lastTime);
	int statePin = me->super.myGPIO->ODR & me->super.GPIOPin;
	if (statePin == 0) { //currently is off
		if (deltaTime > me->timeToOff) {
			GPIO_WriteBit(me->super.myGPIO, me->super.GPIOPin, 1);
			me->lastTime = currentTime;
		}
	} else if (statePin != 0) { //currently is on
		if (deltaTime > me->timeToOn) {
			GPIO_WriteBit(me->super.myGPIO, me->super.GPIOPin, 0);
			me->lastTime = currentTime;
		}
	}
}
