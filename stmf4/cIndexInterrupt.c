#include "cIndexInterrupt.h"
#include "myLib/systemTimer.h"
#include <stdlib.h>
#include <string.h>
#include "deviceConfig.h"

IndexInterrupt indexInterrupt0;

void IndexInterruptStructInit(IndexInterrupt * me) {
	memset(me, 0, sizeof(IndexInterrupt));
}
void IndexInterruptConfig0(IndexInterrupt * me) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	me->id = 0;
	/*
	 * 5d/360d*1028ppr*30 = 428 pulses
	 * 1rpm/60*30*1028ppr = 514;
	 * t = 428/514 = 831000
	 */
	me->timeOut = INDEX_PULSE_TIMEOUT;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure Button pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect Button EXTI Line to Button GPIO Pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);

	EXTI_DeInit();
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
IndexInterrupt* GetIndexInterruptPtr0() {
	return &indexInterrupt0;
}
/*
 * need to be call frequenctly
 */
int ReadIndexFlag(IndexInterrupt * me) {
	unsigned int curentTime, deltaTime;
	if (me->hitFlag == 1) {
		curentTime = GetSystemTimer();
		deltaTime = abs(curentTime - me->timeWhenHitIndex);
		if (deltaTime > me->timeOut) {
			me->hitFlag = 0;
		}
	}
	return me->hitFlag;
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
		if(indexInterrupt0.hitFlag == 0){
			indexInterrupt0.hitFlag = 1;
			indexInterrupt0.timeWhenHitIndex = GetSystemTimer();
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}
