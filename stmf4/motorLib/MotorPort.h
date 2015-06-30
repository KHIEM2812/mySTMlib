/* Khiem
 * Dec 2, 2014
 * project: servo motor
 * description: to interface with the motor controller hardware
 *
 */

#ifndef _motorPort_h
#define _motorPort_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "deviceConfig.h"

typedef struct {
	int isEnable;
	uint16_t motorDirectionPin[2];
	u32 speedAddr;
	u32 speedAddr2;
	GPIO_TypeDef *motorDirectionPort[2]; //0==index for forward, 1==index for reverse
	volatile uint32_t * motorTIM;
	volatile uint32_t * motorDAC;
} MotorPort;

void MotorPortStructInit(MotorPort * me);

void MotorPort0Config(MotorPort * me);
void MotorPort1Config(MotorPort * me);
void MotorPort2Config(MotorPort * me);
void MotorPortConfigForMAT(MotorPort * me);
void MotorPortStructInit(MotorPort * me);

int IsMotorEnable(MotorPort * me);
void EnableMotor(MotorPort * me);
void DisableMotor(MotorPort * me);

void SetRotate(MotorPort * me, int iValue); //Port--> setPWM
void SetStop(MotorPort * me); //Port
void SetSoftStop(MotorPort * me);
inline void SetRotatePWM(MotorPort * me, int setSpeed);
#endif
