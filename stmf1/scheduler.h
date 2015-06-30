#ifndef _scheduler_h
#define _scheduler_h

#include "STM32_Nucleo/stm32f1xx/stm32f1xx_nucleo.h"
#include "timer.h"

#define NUM_TASKS	5

typedef struct {
	void (*task)(void); // Pointer to the task function.
	s32 period; // Period to execute with.
	s32 lastExeTick; // Delay before first call.
} task_type;

void Create_Task(void (*function_pointer)(void), uint32_t period,
		uint32_t delay);
void Dispatch_Tasks(void);

#endif
