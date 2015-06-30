#include "scheduler.h"

task_type tasks[NUM_TASKS];
s32 elapsedTicks = 0;

void Create_Task(void (*function_pointer)(void), uint32_t period,
		uint32_t lastExeTick) {
	static uint32_t index = 0;
//	assert(index < NUM_TASKS);

	tasks[index].task = function_pointer;
	tasks[index].period = period;
	// Avoid underflow in the dispatcher.
	tasks[index].lastExeTick = lastExeTick + 1;
	++index;
}

void Dispatch_Tasks(void) {
	u8 i = 0;
	for (; i < NUM_TASKS; i++) {
		elapsedTicks = (s32)timerMicros();
		s32 duration = (elapsedTicks - tasks[i].lastExeTick);
		if (duration >= tasks[i].period) {
			tasks[i].lastExeTick = elapsedTicks;
			tasks[i].task(); // Execute the task!
		}
	}
}
