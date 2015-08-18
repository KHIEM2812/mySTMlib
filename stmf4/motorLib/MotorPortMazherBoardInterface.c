#include "motorLib/MotorPortMazherBoardInterface.h"
#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "deviceConfig.h"
#include "myLib/Assert.h"
#include "motorLib/MotorPort.h"
#include "motorLib/Encoder.h"
#include "motorLib/pid.h"
#include "motorLib/MotorController.h"
#include "cAbsEncoder.h"

//pwm from -1050 to 1050
void SetRotateMotorMazherBoardInterface(MotorPort * me, int iValue) {
	if (iValue > 0) {
		GPIO_WriteBit(me->motorControlPort[0], me->motorControlPin[0], Bit_SET);
		*me->motorTIM = iValue;
	} else {
		GPIO_WriteBit(me->motorControlPort[0], me->motorControlPin[0],
				Bit_RESET);
		*me->motorTIM = -iValue;
	}
}
void EncoderPortPA67ConfigMazherBoardInterface(Encoder* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_SetAutoreload(TIM3, 0xFFFF);
	TIM_SetCounter(TIM3, 0);
//	TIM_Cmd(TIM3, ENABLE);
	me->encoderTIM = TIM3;
	me->encoderAddr = (unsigned int) &TIM3->CNT;
	EnableEncoder(me); //	TIM_Cmd(TIM3, ENABLE);
}
void MotorOnePortConfig0(MotorPort * me) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //pwm clock

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //sr pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // Phase Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // Reset Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//PWM configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //pwm high pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //pwm low pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = MAX_PWM; //1050
	TIM_TimeBaseStructure.TIM_Prescaler = 3; //clk@21MHz = 84MHz/4
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;

	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);

	TIM_Cmd(TIM5, ENABLE);

//ff pin configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // ff pin1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // ff pin2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Enable Synchronous Rectification
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);

	//Set direction
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);

	//Set PWM LO to enable constantly resulting in a drive/brake configuration
	GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);

	//pull reset high to enable motor
	GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
}
void MotorOnePortConfigMazherBoardInterface(MotorPort * me) {
	MotorOnePortConfig0(me);
	me->motorDAC = 0;
	me->motorTIM = &(TIM5->CCR3);
	me->motorControlPort[0] = GPIOA; //direction pin
	me->motorControlPin[0] = GPIO_Pin_8;
	me->motorControlPort[1] = GPIOC; //reset pin??
	me->motorControlPin[1] = GPIO_Pin_3;
	SetRotateMotorMazherBoardInterface(me, 0);
}

void EncoderPortPB67ConfigMazherBoardInterface(Encoder* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	//configure encoder
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_SetAutoreload(TIM4, 0xFFFF);
	TIM_SetCounter(TIM4, 0);

	//	TIM_Cmd(TIM4, ENABLE);
	me->encoderTIM = TIM4;
	me->encoderAddr = (unsigned int) &TIM4->CNT;
	EnableEncoder(me); //	TIM_Cmd(TIM4, ENABLE);
}
void MotorTwoPortConfig1(MotorPort * me) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //Configure SR Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //Configure Phase Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //Configure Reset Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//pwm configuration
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //CHANNEL2_PWM_HI_PINS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //CHANNEL2_PWM_LO_PINS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;
	TIM_TimeBaseStructure.TIM_Prescaler = 3;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;

	TIM_OC1Init(TIM12, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);

	TIM_Cmd(TIM12, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //CHANNEL2_FF1_PIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //CHANNEL2_FF2_PIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//configure debug pin
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //Debug Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Enable Synchronous Rectification
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);

	//Set Phase Pin
	GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET);

	//Set PWM LO to enable constantly resulting in a drive/brake configuration
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);

	//pull high to enable motor
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, RESET);
}
void MotorTwoPortConfigMazherBoardInterface(MotorPort * me) {
	MotorTwoPortConfig1(me);
	me->motorDAC = 0;
	me->motorTIM = &(TIM12->CCR1);
	me->motorControlPort[0] = GPIOA; //direction pin
	me->motorControlPin[0] = GPIO_Pin_9;
	me->motorControlPort[1] = GPIOA; //reset pin??
	me->motorControlPin[1] = GPIO_Pin_0;
	SetRotateMotorMazherBoardInterface(me, 0);
}

void EnableMotorMazherBoardInterface(MotorPort* me) {
	me->isEnable = 1;
	GPIO_WriteBit(me->motorControlPort[1], me->motorControlPin[1], Bit_SET);
}
void DisableMotorMazherBoardInterface(MotorPort* me) {
	me->isEnable = 0;
	GPIO_WriteBit(me->motorControlPort[1], me->motorControlPin[1], Bit_RESET);
}
int IsEnableMotorMazherBoardInterface(MotorPort* me) {
	return me->isEnable;
}

void SetSoftStopMotorMazherBoardInterface(MotorPort * me) {
	SetRotateMotorMazherBoardInterface(me, 0);
}

void MotorOneControllerConfigMazherBoardInterface(MotorController* me) {
	REQUIRE(me);
	//init motorData
	MotorData* motorData = malloc(sizeof(MotorData));
	REQUIRE(motorData!=0);
	MotorDataStructInitialize(motorData);
	motorData->motorCountsPerRev = ABSENCCOUNTPERREV;
	motorData->motorPositionDeadband = MOTOR1_POS_DEATHBAND;
	motorData->motorVelocityZeroDeathBand = MOTOR1_VELOCITY_ZERO;
	motorData->motorAccelerationLimit = MOTOR1_ACCELERATION_LIMIT;
	motorData->motorVelocityLimit = MOTOR1_VELOCITY_RPM_LIMIT;
	motorData->motorPwmChangeLimit = MOTOR1_PWMCHANGELIMIT;
	motorData->motorControlLoopTicks = MOTOR1_CONTROLLOOP_TICKS;
	me->motorData = motorData;

	//init motorPort
	MotorPort* motorPort = malloc(sizeof(MotorPort));
	REQUIRE(motorPort!=0);
	MotorPortStructInit(motorPort);
	MotorOnePortConfigMazherBoardInterface(motorPort);
	me->motorPort = motorPort;

	//init motorEncoder
	Encoder* motorEncoder = malloc(sizeof(Encoder));
	REQUIRE(motorEncoder!=0);
	EncStructInit(motorEncoder);
	EncoderPortPA67ConfigMazherBoardInterface(motorEncoder);
	me->motorEncoder = motorEncoder;

	//init pid
	pidStruct_t* veloPID = malloc(sizeof(pidStruct_t));
	REQUIRE(veloPID!=0);
	PIDStructInit(veloPID);
	PIDConfig(veloPID, MOTOR1_VELOCITY_P, MOTOR1_VELOCITY_I, MOTOR1_VELOCITY_D,
			MOTOR1_VELOCITY_F, MOTOR1_VELOCITY_MAX_P, MOTOR1_VELOCITY_MAX_I,
			MOTOR1_VELOCITY_I_STATE_MAX, MOTOR1_VELOCITY_MAX_D,
			MOTOR1_VELOCITY_MAX_O, MOTOR1_VELOCITY_DELKP, MOTOR1_VELOCITY_KZ);
	me->motorVelocityPID = veloPID;

	pidStruct_t* posPID = malloc(sizeof(pidStruct_t));
	REQUIRE(posPID!=0);
	PIDStructInit(posPID);
	PIDConfig(posPID, MOTOR1_POSITION_P, MOTOR1_POSITION_I, MOTOR1_POSITION_D,
			MOTOR1_POSITION_F, MOTOR1_POSITION_MAX_P, MOTOR1_POSITION_MAX_I,
			MOTOR1_POSITION_I_STATE_MAX, MOTOR1_POSITION_MAX_D,
			MOTOR1_POSITION_MAX_O, MOTOR1_POSITION_DELKP, MOTOR1_POSITION_KZ);
	me->motorPositionPID = posPID;

	me->enableFlag = 1; //the flag helps to make sure the pointer points to the legit region
	EnableMotor(me->motorPort);
}
void MotorTwoControllerConfigMazherBoardInterface(MotorController* me) {
	REQUIRE(me);
	//init motorData
	MotorData* motorData = malloc(sizeof(MotorData));
	REQUIRE(motorData!=0);
	MotorDataStructInitialize(motorData);
	motorData->motorCountsPerRev = MOTOR2_COUNTS_PERREV;
	motorData->motorPositionDeadband = MOTOR2_POS_DEATHBAND;
	motorData->motorVelocityZeroDeathBand = MOTOR2_VELOCITY_ZERO;
	motorData->motorAccelerationLimit = MOTOR2_ACCELERATION_LIMIT;
	motorData->motorVelocityLimit = MOTOR2_VELOCITY_RPM_LIMIT;
	motorData->motorPwmChangeLimit = MOTOR2_PWMCHANGELIMIT;
	motorData->motorControlLoopTicks = MOTOR2_CONTROLLOOP_TICKS;
	me->motorData = motorData;

	//init motorPort
	MotorPort* motorPort = malloc(sizeof(MotorPort));
	REQUIRE(motorPort!=0);
	MotorPortStructInit(motorPort);
	MotorTwoPortConfigMazherBoardInterface(motorPort);
	me->motorPort = motorPort;

	//init motorEncoder
	Encoder* motorEncoder = malloc(sizeof(Encoder));
	REQUIRE(motorEncoder!=0);
	EncStructInit(motorEncoder);
	EncoderPortPB67ConfigMazherBoardInterface(motorEncoder);
	me->motorEncoder = motorEncoder;

	//init pid
	pidStruct_t* veloPID = malloc(sizeof(pidStruct_t));
	REQUIRE(veloPID!=0);
	PIDStructInit(veloPID);
	PIDConfig(veloPID, MOTOR2_VELOCITY_P, MOTOR2_VELOCITY_I, MOTOR2_VELOCITY_D,
			MOTOR2_VELOCITY_F, MOTOR2_VELOCITY_MAX_P, MOTOR2_VELOCITY_MAX_I,
			MOTOR2_VELOCITY_I_STATE_MAX, MOTOR2_VELOCITY_MAX_D,
			MOTOR2_VELOCITY_MAX_O, MOTOR2_VELOCITY_DELKP, MOTOR2_VELOCITY_KZ);
	me->motorVelocityPID = veloPID;

	pidStruct_t* posPID = malloc(sizeof(pidStruct_t));
	REQUIRE(posPID!=0);
	PIDStructInit(posPID);
	PIDConfig(posPID, MOTOR2_POSITION_P, MOTOR2_POSITION_I, MOTOR2_POSITION_D,
			MOTOR2_POSITION_F, MOTOR2_POSITION_MAX_P, MOTOR2_POSITION_MAX_I,
			MOTOR2_POSITION_I_STATE_MAX, MOTOR2_POSITION_MAX_D,
			MOTOR2_POSITION_MAX_O, MOTOR2_POSITION_DELKP, MOTOR2_POSITION_KZ);
	me->motorPositionPID = posPID;

	me->enableFlag = 1; //the flag helps to make sure the pointer points to the legit region
	EnableMotor(me->motorPort);
}

void AbsEncoderPortConfigMazherBoardInterface() {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure SPI1 pins: SCK and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure MISO as Input Floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure SPI1 pins: CS or NSS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Connect SPI pins to Alternate Function
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

	/* Enable SPI3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStructure);

	/* Enable SPI1 */
	SPI_Cmd(SPI3, ENABLE);
}
int GetAbsEncoderCountsMazherBoardInterface(cAbsEncoder* me) {
	GPIO_ResetBits(me->encPort->SPINSSPort, me->encPort->pinSPINSS);
	while (SPI_I2S_GetFlagStatus(me->encPort->SPIx, SPI_I2S_FLAG_TXE) == RESET)
		;
	SPI_I2S_SendData(me->encPort->SPIx, 0x0000);
	while (SPI_I2S_GetFlagStatus(me->encPort->SPIx, SPI_I2S_FLAG_RXNE) == RESET)
		;
	me->encoderValue = (int) (SPI_I2S_ReceiveData(me->encPort->SPIx) >> 5);
	GPIO_SetBits(me->encPort->SPINSSPort, me->encPort->pinSPINSS);
	return me->encoderValue;
}

