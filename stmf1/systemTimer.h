/* Khiem
 * Dec 2, 2014
 * project:general
 * description: manage the system clock
 *
 */

#ifndef _systemTimer_h
#define _systemTimer_h

#include "STM32F4-Discovery/stm32f4_discovery.h"

void SystemTimerInit(int SystemTimerFrequency);
unsigned long GetSystemTimer(void);
void SetSystemTimer(unsigned int iSystemCounter);
void ResetSystemTimerAndDisable();
int IsSystemTimerEnable();
void EnableSystemTimer(void);
void DisableSystemTimer(void);

#endif
