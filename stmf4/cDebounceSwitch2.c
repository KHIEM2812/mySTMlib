#include "cDebounceSwitch2.h"
#include "myLib/cUart.h"
void cDebounceBtnInit(cDebounceBtn *me) {
	memset(me, 0, sizeof(cDebounceBtn));
}

void cDebounceBtnConfig1(cDebounceBtn *me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the BUTTON Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure Button pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	me->thisBtnPin = GPIO_Pin_0;
	me->thisBtnPort = GPIOA;
	me->keyPressed = RELEASED;
	me->keyIntegrator = 0;
}

void cDebounceBtnProcess(cDebounceBtn *me) {
	unsigned char rawState = GPIO_ReadInputDataBit(me->thisBtnPort,
			me->thisBtnPin);
	if(me->thisBtnPin==16){
		int i =0;
	}
	if (rawState == (int) RELEASED) {
		if (me->keyIntegrator > 0) {
			me->keyIntegrator--;
		}
	} else if (me->keyIntegrator < MAXIMUM)
		me->keyIntegrator++;

	if (me->keyIntegrator == 0)
		me->keyPressed = RELEASED;
	else if (me->keyIntegrator >= MAXIMUM) {
		me->keyPressed = PRESSED;
		me->keyIntegrator = MAXIMUM; /* defensive code if integrator got corrupted */
	}
}

void cDebounceBtnDebounce(cDebounceBtn *me) {
	unsigned long tempTimer = GetSystemTimer();
	int tempTimer1 = tempTimer - me->lastTime;
	if (tempTimer1 > CHECK_MSEC) {
		cDebounceBtnProcess(me);
		me->lastTime = tempTimer;
	}
}

int ReadDebounceBtn(cDebounceBtn *me){
	return (int)me->keyPressed;
}
