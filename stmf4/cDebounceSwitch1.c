#include "cDebounceSwitch1.h"

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
	me->keyFloat = 0;
	me->keyPressed = RELEASED;
}

void cDebounceBtnProcess(cDebounceBtn *me) {
	me->keyPressed = me->lastKeyPressed;
	unsigned char rawState = GPIO_ReadInputDataBit(me->thisBtnPort,
			me->thisBtnPin);
	me->keyFloat = (me->keyFloat * 9 + rawState * 10) / 10;
	if (me->keyFloat > 5) {
		me->keyPressed = PRESSED;
	} else {
		me->keyPressed = RELEASED;
	}
}

void cDebounceBtnDebounce(cDebounceBtn *me) {
	unsigned long tempTimer = GetSystemTimer();
	int tempTimer1 = tempTimer - me->lastTime;
	if (tempTimer1 > CHECK_MSEC * 10) {
		cDebounceBtnProcess(me);
		me->lastTime = tempTimer;
	}
}
