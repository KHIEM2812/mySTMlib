/* Khiem
 * Dec 2, 2014
 * project:exoSkeleton
 * description: test if the encoders work correctly
 * 		encoder 0 : attached with linear pot
 * 		encoder 1 : attached with motor
 *
 */

#ifndef _tEncoder_h
#define _tEncoder_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "FSM.h"
#include "systemTimer.h"
#include "cUart.h"
#include "Assert.h"
#include "encoder.h"
#include "MotorPort.h"
#include "utility.h"
#include "../cAlienCatherPeripheral.h"

void TEncoderConfig();
void DisplayEnc0Counts();
void DisplayEnc1Counts();
void TEncoder0Features();

void TEnc0BehaviorConfig();
void TEnc0Behavior();
void TEnc1BehaviorConfig();
void TEnc1Behavior();
#endif
