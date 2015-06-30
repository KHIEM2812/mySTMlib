#include "systemTimer.h"

NVIC_InitTypeDef NVIC_InitStructure;

uint8_t SystemTimer_Initialized = 0;
volatile unsigned int systemCounter = 0;

// must be called at least once every 65536 us in order for this strategy to work
unsigned long GetSystemTimer(void) {
	return systemCounter;
}

void SetSystemTimer(unsigned int iSystemCounter) {
	systemCounter = iSystemCounter;
}

void ResetSystemTimerAndDisable() {
	SystemTimer_Initialized = 0;
}

int IsSystemTimerEnable() {
	return (int) SystemTimer_Initialized;
}

void EnableSystemTimer(void) {
	if (!SystemTimer_Initialized) {
		return;
	}
	/* Enable systick interrupts, useful when you wakeup from sleep mode */
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void DisableSystemTimer(void) {
	/* Disable systick, useful when you go to sleep mode */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void SystemTimerInit(int SystemTimerFrequency) {
	if (SystemTimer_Initialized) {
		return;
	}
	if (SysTick_Config(SystemCoreClock / SystemTimerFrequency)) {
		/* Capture error */
		while (1)
			;
	}
	SystemTimer_Initialized = 1;
}

void SysTick_Handler(void) {
	systemCounter++;
}
