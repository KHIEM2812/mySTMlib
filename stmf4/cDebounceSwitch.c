#include "cDebounceSwitch.h"

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
	me->lastKeyPressed = RELEASED;
	me->counts = RELEASE_MSEC / CHECK_MSEC;
}

void cDebounceBtnProcess(cDebounceBtn *me) {
	me->keyChanged = 0;
	me->keyPressed = me->lastKeyPressed;
	unsigned char rawState = GPIO_ReadInputDataBit(me->thisBtnPort,
			me->thisBtnPin);

	if (rawState == me->lastKeyPressed) {
		//set the timer which allows a change from the current state
		if (!me->lastKeyPressed) {
			me->counts = RELEASE_MSEC / CHECK_MSEC;
		} else {
			me->counts = PRESS_MSEC / CHECK_MSEC;
		}
	} else {
		//key has changed - wait for new state to become stable
		if (--me->counts == 0) {
			me->lastKeyPressed = rawState;
			me->keyPressed = me->lastKeyPressed;
			me->keyChanged = 1;
			//and reset timer
			if (!me->keyPressed) {
				me->counts = RELEASE_MSEC / CHECK_MSEC;
			} else {
				me->counts = PRESS_MSEC / CHECK_MSEC;
			}
		}
	}
}

void cDebounceBtnDebounce(cDebounceBtn *me) {
	unsigned long tempTimer = GetSystemTimer();
	int tempTimer1 = tempTimer - me->lastTime;
	if (tempTimer1 > CHECK_MSEC * 1000) {
		cDebounceBtnProcess(me);
		me->lastTime = tempTimer;
	}
}
