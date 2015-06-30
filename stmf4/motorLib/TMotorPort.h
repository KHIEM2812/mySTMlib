/* Khiem
 * Dec 2, 2014
 * project:exoSkeleton
 * description: test if the MotorPort work correctly
 *
 */

#ifndef _tMotorPort_h
#define _tMotorPort_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "myLib/FSM.h"
#include "myLib/systemTimer.h"
#include "myLib/cUart.h"
#include "myLib/Assert.h"
#include "motorLib/MotorPort.h"
#include "myLib/utility.h"
#include <unistd.h>

void TMotorPortConfig();
void TSetRotate();
void TSetStop();
void TSetSoftStop();
void TEnableMotor();
void TDisableMotor();

#endif
