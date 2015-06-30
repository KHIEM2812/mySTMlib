/* Khiem
 * Dec 2, 2014
 * project:exoSkeleton
 * description: test if the MotorController work correctly
 *
 */
#ifndef _tMotorController_h
#define _tMotorController_h

#include <stdio.h>
#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "myLib/FSM.h"
#include "myLib/systemTimer.h"

#include "myLib/Assert.h"

#include "myLib/utility.h"

void TestMotorControllerConfig();
//test stop, set speed, ...
void TestMCConsistentBtwnMotorPortNEncoder();
void TestMCVeloFBControl();
void TestMCPosFBControl();
void TestMCMixFBControl();
#endif
