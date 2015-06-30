#include "encoder.h"

void EncStructInit(encoder_t* me) {
	me->isEnable = 0;
	me->prevEncoderCount = 0;
	me->currEncoderCount = 0;
	me->deltaEncoderCount = 0;
	me->accumEncoderCounts = 0;
	me->encoderAddr = 0;
	me->encoderTIM = 0;
}

//use tim1,cc1
void Enc3Config(encoder_t* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //channel1
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //channel2
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM3, 0xffff);
	TIM_SetCounter(TIM3, 0);
	me->encoderTIM = TIM3;
	me->encoderAddr = (u32) &TIM3->CNT;
	EnableEncoder(me);
}
void Enc0Config(encoder_t* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //channel1
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //channel2
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM3, 0xffff);
	TIM_Cmd(TIM3, ENABLE);
	me->encoderTIM = TIM3;
	me->encoderAddr = (u32) &TIM3->CNT;
	EnableEncoder(me);
}
//use tim8,cc1
void Enc1Config(encoder_t* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //channel1
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //channel2
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM4, 0xffff);
	TIM_SetCounter(TIM4, 0);
	me->encoderTIM = TIM4;
	me->encoderAddr = (u32) &TIM4->CNT;
	EnableEncoder(me);
}

void DisableEncoder(encoder_t * me) {
	TIM_Cmd(me->encoderTIM, DISABLE);
	me->isEnable = 0;
}

void EnableEncoder(encoder_t * me) {
	TIM_Cmd(me->encoderTIM, ENABLE);
	me->isEnable = 1;
}

int IsEncoderEnable(encoder_t * me) {
	return me->isEnable;
}

//to avoid overflow
void SetEncoder(encoder_t * me, s32 encoderCounter) {
	DisableEncoder(me);
	me->accumEncoderCounts = encoderCounter;
	me->currEncoderCount = 0;
	me->prevEncoderCount = 0;
	me->deltaEncoderCount = 0;
	TIM_SetCounter(me->encoderTIM, 0);
	EnableEncoder(me);
}

//Port, need to be called frequently
//ret:encoderCounter
s32 GetEncoderCounts(encoder_t * me) {
	me->currEncoderCount = (s16) (*((u16*) me->encoderAddr));
	me->deltaEncoderCount = me->currEncoderCount - me->prevEncoderCount;
	me->prevEncoderCount = me->currEncoderCount;
	me->accumEncoderCounts += (s32) me->deltaEncoderCount;
	return me->accumEncoderCounts;
}

void ResetEncoder(encoder_t * me) {
	DisableEncoder(me);
	me->prevEncoderCount = 0;
	me->currEncoderCount = 0;
	me->deltaEncoderCount = 0;
	me->accumEncoderCounts = 0;
	TIM_SetCounter(me->encoderTIM, 0);
	GetEncoderCounts(me);
	EnableEncoder(me);
}

