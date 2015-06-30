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
	s32 accumEncoderCounts;
	u32 encoderAddr;
	TIM_TypeDef * encoderTIM;
} encoder_t;

void Enc0Config(encoder_t* me);
void Enc1Config(encoder_t* me);

void EncStructInit(encoder_t* me);

void DisableEncoder(encoder_t * me);
void EnableEncoder(encoder_t * me);
int IsEncoderEnable(encoder_t * me);

//it s not a good practice cuz not follow inverse control principle
void UpdateEncoder(encoder_t * me);

void SetEncoder(encoder_t * me, s32 encoderCounter);
s32 GetEncoderCounts(encoder_t * me);
void ResetEncoder(encoder_t * me);

#endif
