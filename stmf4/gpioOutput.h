#ifndef _gpioOutput_h
#define _gpioOutput_h

/* Khiem
 * Jun 30, 2015
 * project: general
 * description: generalize output n patternOutpur
 */


#include "STM32F4-Discovery/stm32f4_discovery.h"

typedef struct OutputPinStruct{
	GPIO_TypeDef *myGPIO;
	uint32_t GPIOPin;
	int type;
}OutputPinStruct ;

typedef struct PatternedOutputPinStruct{
	OutputPinStruct super;
	int timeToOn;
	int timeToOff;
	uint32_t lastTime;
}PatternedOutputPinStruct ;

void OutputPinInitStruct(OutputPinStruct *me, int type);
void OffOutputPin(OutputPinStruct *me);
void OnOutputPin(OutputPinStruct *me);
void SetPatternOutputPin(PatternedOutputPinStruct *me, int timeOn, int timeOff);
void OutputPinPatternRun(PatternedOutputPinStruct *me);



#endif
