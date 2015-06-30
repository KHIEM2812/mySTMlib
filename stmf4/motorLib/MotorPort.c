#include "MotorPort.h"

#define DHR12R1_OFFSET             ((uint32_t)0x00000008)
#define DHR12R2_OFFSET             ((uint32_t)0x00000014)

void MotorPortStructInit(MotorPort * me) {
	int i = 0;
	me->isEnable = DISABLE;
	i = 1;
	do {
		me->motorDirectionPin[i] = 0;
		me->motorDirectionPort[i] = 0;
	} while (i-- != 0);

	me->motorTIM = 0;
	me->motorDAC = 0;
}
/************************************************************************************/
/******************************** MOTOR CONFIGURATION *****************************************/
/**
 * @brief  PWM without DMA
 * @param  None
 * @retval None
 * @note
 */
inline void Tim1PWMConfig1() {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	uint16_t uhPrescalerValue = (uint16_t)((SystemCoreClock) / 30000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000;
	TIM_TimeBaseStructure.TIM_Prescaler = uhPrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}
inline void Tim8Config1() {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	uint16_t uhPrescalerValue = (uint16_t)((SystemCoreClock) / 1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000;
	TIM_TimeBaseStructure.TIM_Prescaler = uhPrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM8, ENABLE);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Disable);

	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);
}

inline void PE2nPE3DirectionConfig1(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
inline void PC0nPC1DirectionConfig1(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

inline void Pin5DAC2Config0(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_StructInit(&DAC_InitStructure);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_2, ENABLE);
}
inline void PB0nPB1DirectionConfig0(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

inline void Pin4DAC1Config2(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_StructInit(&DAC_InitStructure);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);
}
inline void PE6nPC13DirectionConfig2(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//for implementation
inline void MotorPort0ConfigInterface0(MotorPort * me) {
	Pin5DAC2Config0();
	PB0nPB1DirectionConfig0();
	me->motorDAC = (uint32_t*) (DAC_BASE + DHR12R2_OFFSET + DAC_Align_12b_R);
	me->motorTIM = 0;
	me->motorDirectionPort[0] = GPIOB;
	me->motorDirectionPin[0] = GPIO_Pin_0;
	me->motorDirectionPort[1] = GPIOB;
	me->motorDirectionPin[1] = GPIO_Pin_1;
	SetRotate(me, 0);
}
inline void MotorPort0ConfigInterface1(MotorPort * me) {
	Tim8Config1();
	PC0nPC1DirectionConfig1();
	me->motorTIM = &(TIM8->CCR1);
	me->motorDirectionPort[0] = GPIOC;
	me->motorDirectionPin[0] = GPIO_Pin_0;
	me->motorDirectionPort[1] = GPIOC;
	me->motorDirectionPin[1] = GPIO_Pin_1;
	SetRotate(me, 0);
}
inline void MotorPort0ConfigInterface11(MotorPort * me) {
	Tim8Config1();
	PC0nPC1DirectionConfig1();
	me->motorDAC = 0;
	me->motorTIM = &(TIM1->CCR1);
	me->motorDirectionPort[1] = GPIOE;
	me->motorDirectionPin[1] = GPIO_Pin_2;
	me->motorDirectionPort[0] = GPIOE;
	me->motorDirectionPin[0] = GPIO_Pin_3;
	SetRotate(me, 0);
}
inline void MotorPort0ConfigInterface2(MotorPort * me) {
	Pin4DAC1Config2();
	PE6nPC13DirectionConfig2();
	me->motorDAC = (uint32_t*) (DAC_BASE + DHR12R1_OFFSET + DAC_Align_12b_R);
	me->motorTIM = 0;
	me->motorDirectionPort[0] = GPIOE;
	me->motorDirectionPin[0] = GPIO_Pin_6;
	me->motorDirectionPort[1] = GPIOC;
	me->motorDirectionPin[1] = GPIO_Pin_13;
	SetRotate(me, 0);
}

void MotorPort0Config(MotorPort * me) {
	MotorPort0ConfigInterface0(me);
}
void MotorPort1Config(MotorPort * me) {
	MotorPort0ConfigInterface1(me);
}
void MotorPort2Config(MotorPort * me) {
	MotorPort0ConfigInterface2(me);
}

void MotorPortConfigForMAT(MotorPort * me) {
	Pin4DAC1Config2();
	PC0nPC1DirectionConfig1();
	me->motorDAC = (uint32_t*) (DAC_BASE + DHR12R1_OFFSET + DAC_Align_12b_R);
	me->motorTIM = 0;
	me->motorDirectionPort[0] = GPIOC;
	me->motorDirectionPin[0] = GPIO_Pin_0;
	me->motorDirectionPort[1] = GPIOC;
	me->motorDirectionPin[1] = GPIO_Pin_1;
	SetRotate(me, 0);
}

inline void EnableMotorInterface0(MotorPort * me) {
	DAC_DMACmd(DAC_Channel_2, ENABLE);
	me->isEnable = 1;
}

inline void DisableMotorInterface0(MotorPort * me) {
	DAC_DMACmd(DAC_Channel_2, DISABLE);
	me->isEnable = 0;
}

/************************************************************************************/
/******************************** MOTOR ROTATE *****************************************/

/* input:
 * 		setSpeed Value: [-1000,1000];*/
inline void SetRotateDACInterface0(MotorPort * me, int setSpeed) {
	setSpeed = 4 * setSpeed;
	if (setSpeed > 0) {
		GPIO_WriteBit(me->motorDirectionPort[0], me->motorDirectionPin[0], SET);
		GPIO_WriteBit(me->motorDirectionPort[1], me->motorDirectionPin[1],
				!SET);
		*me->motorDAC = (uint16_t)(setSpeed);
	} else if (setSpeed == 0) {
		GPIO_WriteBit(me->motorDirectionPort[0], me->motorDirectionPin[0],
				!SET);
		GPIO_WriteBit(me->motorDirectionPort[1], me->motorDirectionPin[1],
				!SET);
		*me->motorDAC = (uint16_t) 0;
	} else {
		GPIO_WriteBit(me->motorDirectionPort[0], me->motorDirectionPin[0],
				!SET);
		GPIO_WriteBit(me->motorDirectionPort[1], me->motorDirectionPin[1], SET);
		*me->motorDAC = (uint16_t)(-setSpeed);
	}
}
inline void SetRotateDAC(MotorPort * me, int setSpeed) {
	SetRotateDACInterface0(me, setSpeed);
}
inline void SetRotatePWM(MotorPort * me, int setSpeed) {
	if (setSpeed > 0) {
		GPIO_WriteBit(me->motorDirectionPort[0], me->motorDirectionPin[0], SET);
		GPIO_WriteBit(me->motorDirectionPort[1], me->motorDirectionPin[1],
				!SET);
		*me->motorTIM = (uint32_t) setSpeed;
	} else if (setSpeed == 0) {
		GPIO_WriteBit(me->motorDirectionPort[0], me->motorDirectionPin[0],
				!SET);
		GPIO_WriteBit(me->motorDirectionPort[1], me->motorDirectionPin[1],
				!SET);
		*me->motorTIM = (uint32_t) 0;
	} else {
		GPIO_WriteBit(me->motorDirectionPort[0], me->motorDirectionPin[0],
				!SET);
		GPIO_WriteBit(me->motorDirectionPort[1], me->motorDirectionPin[1], SET);
		*me->motorTIM = (uint32_t) (-setSpeed);
	}
}

inline void SetSoftStopInterface0(MotorPort * me) {
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, !SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, !SET);
	SetRotate(me, 0);
}
inline void SetSoftStopInterface1(MotorPort * me) {
	SetRotate(me, 0);
}

void SetStop(MotorPort * me) {

}

int IsMotorEnable(MotorPort * me) {
	return me->isEnable;
}
void EnableMotor(MotorPort * me) {
	EnableMotorInterface0(me);
}
void DisableMotor(MotorPort * me) {
	SetSoftStop(me);
	DisableMotorInterface0(me);
}
//get any Value from -1000 to 1000
void SetRotate(MotorPort * me, int iValue) {
	if (me->motorTIM != 0) {
		SetRotatePWM(me, iValue);
	} else if (me->motorDAC != 0) {
		SetRotateDAC(me, iValue);
	}
}
void SetSoftStop(MotorPort * me) {
	SetSoftStopInterface0(me);
}
