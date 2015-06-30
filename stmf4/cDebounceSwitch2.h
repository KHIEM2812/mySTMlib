/*
 * @author:khiem
 * @Date: 17/3/2015
 * @describe: using integrator algorithm to debound switch
 * http://www.kennethkuhn.com/electronics/debounce.c
 *
 real signal 0000111111110000000111111100000000011111111110000000000111111100000

 corrupted   0100111011011001000011011010001001011100101111000100010111011100010
 integrator  0100123233233212100012123232101001012321212333210100010123233321010
 output      0000001111111111100000001111100000000111111111110000000001111111000
 */

#ifndef cDebounceSwitch_h
#define cDebounceSwitch_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include <string.h>
#include "../stmf4/systemTimer.h"

#define CHECK_MSEC			100
#define DEBOUNCE_TIME       0.3
#define SAMPLE_FREQUENCY    10
#define MAXIMUM         	(DEBOUNCE_TIME*1000 / CHECK_MSEC)

typedef enum {
	PRESSED = 1, RELEASED = 0
} BtnState;

typedef struct {
	int keyIntegrator;
	BtnState keyPressed; //hold the debounce state of btn
	unsigned long lastTime;

	uint16_t thisBtnPin;
	GPIO_TypeDef* thisBtnPort;

} cDebounceBtn;

void cDebounceBtnInit(cDebounceBtn *me);
void cDebounceBtnConfig1(cDebounceBtn *me);
void cDebounceBtnDebounce(cDebounceBtn *me);
int ReadDebounceBtn(cDebounceBtn *me);
#endif
