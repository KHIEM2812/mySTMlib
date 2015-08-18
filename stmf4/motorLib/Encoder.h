/* Khiem
 * Dec 2, 2014
 * project: for servo motor
 * description: configure encoder 0, encoder 1
 *		enc0: b4,b5,tim3
 *		enc1: b6,b7,tim4
 */

#ifndef _encoder_h
#define _encoder_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "deviceConfig.h"

typedef struct {
	int isEnable;
	s16 prevEncoderCount;
	s16 currEncoderCount;
	s16 deltaEncoderCount;
	int accumEncoderCounts;
	unsigned int encoderAddr;
	TIM_TypeDef * encoderTIM;
} Encoder;

void Enc0Config(Encoder* me);
void Enc1Config(Encoder* me);

void EncStructInit(Encoder* me);

void DisableEncoder(Encoder * me);
void EnableEncoder(Encoder * me);
int IsEncoderEnable(Encoder * me);

//it s not a good practice cuz not follow inverse control principle
void UpdateEncoder(Encoder * me);

void SetEncoder(Encoder * me, int encoderCounter);
int GetEncoderCounts(Encoder * me);
void ResetEncoder(Encoder * me);

#endif
