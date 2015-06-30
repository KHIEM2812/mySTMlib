#include "TestDeboundSwitch2.h"

cDebounceBtn myBtn;

void TestDeboundSwitchConfig() {
	cDebounceBtnInit(&myBtn);
	cDebounceBtnConfig1(&myBtn);
}
void TestDeboundSwitchRun() {
	cDebounceBtnDebounce(&myBtn);
	if (myBtn.keyPressed) {
		STM_EVAL_LEDOn(LED3);
	} else {
		STM_EVAL_LEDOff(LED3);
	}
}
