#ifndef cDebounceSwitch_h
#define cDebounceSwitch_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include <string.h>

#define CHECK_MSEC		5//read hardware every 5ms
#define PRESS_MSEC		10//Stable time bf registering pressed
#define RELEASE_MSEC	100//Stable time bf registering released
typedef enum {
	PRESSED = 1, RELEASED = 0
} BtnState;

typedef struct {
	int keyChanged;
	BtnState keyPressed, lastKeyPressed; //hold the debounce state of btn
	int pressed_ms, released_ms;
	int counts;
	unsigned long lastTime;
	uint16_t thisBtnPin;
	GPIO_TypeDef* thisBtnPort;

} cDebounceBtn;

void cDebounceBtnInit(cDebounceBtn *me);
void cDebounceBtnConfig1(cDebounceBtn *me);
void cDebounceBtnDebounce(cDebounceBtn *me);
#endif
