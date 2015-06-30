#include "pid.h"
#include <stdlib.h>
#include "myLib/cUart.h"
void PIDStructInit(pidStruct_t* me) {
	me->setPoint = 0;
	me->dState = 0;
	me->iState = 0;
	me->iStateMax = 0;
	me->iGain = 0;
	me->pGain = 0;
	me->dGain = 0;
	me->fGain = 0;
	me->pMax = 0;
	me->iMax = 0;
	me->dMax = 0;
	me->oMax = 0;
	me->pTrim = 0;
	me->iTrim = 0;
	me->dTrim = 0;
	me->fTrim = 0;
	me->pv_1 = 0;
	me->co_1 = 0;
	me->pTerm_1 = 0;
	me->iTerm_1 = 0;
	me->dTerm_1 = 0;
	me->sp_1 = 0;
}

void PIDConfig(pidStruct_t* me, float p, float i, float d, float f, float pMax,
		float iMax, float iStateMax, float dMax, float oMax, int pTrim,
		int iTrim, int dTrim, int fTrim) {

	me->pMax = pMax;
	me->iMax = iMax;
	me->iStateMax = iStateMax;
	me->dMax = dMax;
	me->oMax = oMax;
	me->pGain = p;
	me->iGain = i;
	me->dGain = d;
	me->fGain = f;
	me->pTrim = pTrim;
	me->iTrim = iTrim;
	me->dTrim = dTrim;
	me->fTrim = fTrim;
}

float PIDUpdate(pidStruct_t *pid, float setpoint, float position) {
	float error;
	float p = pid->pGain;
	float i = pid->iGain;
	float d = (pid->dGain) ? pid->dGain : 0.0f;
	float f = (pid->fGain) ? pid->fGain : 1.0f;

	error = setpoint - position;

	// calculate the proportional term
	pid->pTerm_1 = p * error;
	if (pid->pTerm_1 > pid->pMax) {
		pid->pTerm_1 = pid->pMax;
	} else if (pid->pTerm_1 < -pid->pMax) {
		pid->pTerm_1 = -pid->pMax;
	}

	// calculate the integral state with appropriate limiting
	pid->iState += error;
	if (pid->iState > pid->iStateMax)
		pid->iState = pid->iStateMax;
	else if (pid->iState < -pid->iStateMax)
		pid->iState = -pid->iStateMax;

	pid->iTerm_1 = i * pid->iState;
	if (pid->iTerm_1 > pid->iMax) {
		pid->iTerm_1 = pid->iMax;
		pid->iState -= error;
	} else if (pid->iTerm_1 < -pid->iMax) {
		pid->iTerm_1 = -pid->iMax;
		pid->iState -= error;
	}

	// derivative
	if (pid->dGain) {
		// uncomment this line if you want the D term to ignore set point changes
		error = -position;

		pid->dTerm_1 = (d * f) * (error - pid->dState);
		pid->dState += f * (error - pid->dState);
		if (pid->dTerm_1 > pid->dMax) {
			pid->dTerm_1 = pid->dMax;
		} else if (pid->dTerm_1 < -pid->dMax) {
			pid->dTerm_1 = -pid->dMax;
		}
	} else {
		pid->dTerm_1 = 0.0f;
	}

	pid->pv_1 = position;
	pid->sp_1 = setpoint;
	pid->co_1 = pid->pTerm_1 + pid->iTerm_1 + pid->dTerm_1;

	if (pid->co_1 > pid->oMax) {
		pid->co_1 = pid->oMax;
	} else if (pid->co_1 < -pid->oMax) {
		pid->co_1 = -pid->oMax;
	}
	return pid->co_1;
}
