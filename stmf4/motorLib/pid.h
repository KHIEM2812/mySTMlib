#ifndef pid_h
#define pid_h
/* Khiem
 * Jul 29, 2015
 * project: motor library
 * description:
 * 		ref: http://www.mstarlabs.com/apeng/techniques/pidsoftw.html
 */

typedef struct {
	//pid state
	float setPoint; // Last setpoint
	float dState, iState;
	float pTerm_1, iTerm_1, dTerm_1;
	float co_1;
	//PID para
	float iGain; // integral gain
	float pGain; // proportional gain
	float dGain; // derivative gain
	float fGain; // low pass filter factor (1 - pole) for derivative gain
	float pMax, iMax, dMax, oMax;
	float iStateMax;
	float delKp; //handle Unbalanced Output Drive
	float Kz; //feed forward compensation
} pidStruct_t;

void PIDStructInit(pidStruct_t* me);
void PIDConfig(pidStruct_t* me, float p, float i, float d, float f, float pMax,
		float iMax, float iStateMax, float dMax, float oMax, float delKp,
		float Kz);
float PIDUpdate(pidStruct_t *pid, float setpoint, float position);

#endif
