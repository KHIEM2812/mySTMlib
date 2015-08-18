#include "pid.h"
#include <stdlib.h>
#include "myLib/cUart.h"
void PIDStructInit(pidStruct_t* me) {
	memset(me, 0, sizeof(pidStruct_t));
}

void PIDConfig(pidStruct_t* me, float p, float i, float d, float f, float pMax,
		float iMax, float iStateMax, float dMax, float oMax, float delKp,
		float Kz) {
	me->pMax = pMax;
	me->iMax = iMax;
	me->iStateMax = iStateMax;
	me->dMax = dMax;
	me->oMax = oMax;
	me->pGain = p;
	me->iGain = i;
	me->dGain = d;
	me->fGain = f;
	me->delKp = delKp;
	me->Kz = Kz;
}

//http://www.mstarlabs.com/apeng/techniques/pidsoftw.html
float PIDUpdate(pidStruct_t *pid, float setpoint, float position) {
	float p = pid->pGain;
	float i = pid->iGain;
	float d = pid->dGain;
	float f = (pid->fGain) ? pid->fGain : 1.0f;
	float error = setpoint - position;
	if (error > 0) {
		error *= pid->delKp;
	} else {
		error /= pid->delKp;
	}
	// calculate the proportional term
	pid->pTerm_1 = p * error;
	if (pid->pTerm_1 > pid->pMax) {
		pid->pTerm_1 = pid->pMax;
	} else if (pid->pTerm_1 < -pid->pMax) {
		pid->pTerm_1 = -pid->pMax;
	}

	// calculate the integral state with appropriate limiting
	pid->iState += error;
	//this integrator Rate Limiting to avoid windup
	if (pid->iState > pid->iStateMax)
		pid->iState = pid->iStateMax;
	else if (pid->iState < -pid->iStateMax)
		pid->iState = -pid->iStateMax;
	//this integrator latching to avoid windup
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
		//TODO: uncomment this line if you want the D term to ignore set point changes
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

	pid->co_1 = pid->pTerm_1 + pid->iTerm_1 + pid->dTerm_1;

	if (pid->co_1 > pid->oMax) {
		pid->co_1 = pid->oMax;
	} else if (pid->co_1 < -pid->oMax) {
		pid->co_1 = -pid->oMax;
	}
	return pid->co_1;
}
