#include "PIDlib.h"
#include "utility.h"
#include "math.h"

#define MIN(x, y) ( ((x)<(y)) ? (x) : (y) )
#define MAX(x, y) ( ((x)>(y)) ? (x) : (y) )
#define BOUNDED(x, minim, maxim) ( MAX((minim),MIN((x),(maxim))) )

float kpFactor = 0, kiFactor = 0, kdFactor = 0;
float error, output;

void InitPIDState(PID_state * state) {
	state->deriv = 0;
	state->intg = 0;
}

void InitPIDPara(PIDParams * params) {
	params->Kp = 0;
	params->Ki = 0;
	params->Kd = 0;
	params->delT = 0;
	params->lowLimit = 0;
	params->highLimit = 0;
	params->antiWinup = 0;
	params->rateLimit = 0;
	params->delKp = 0;
	params->setPoint = 0;
}
/*SetTunings: set 3 main parameters for PID loop
 * Kp Loop gain parameter
 * Ki Integrator gain parameter
 * Kd Differentiator gain parameter
 * */

void SetTunings(PIDParams *pidParaInput, float Kp, float Ki, float Kd) {
	pidParaInput->Kp = Kp;
	pidParaInput->Ki = Ki;
	pidParaInput->Kd = Kd;
}

/*SetLimits: set Limit to which output converters can respond correctly
 * lowLimit
 * highLimit
 * */

void SetLimits(PIDParams *pidParaInput, float lowLimit, float highLimit) {
	pidParaInput->lowLimit = lowLimit;
	pidParaInput->highLimit = highLimit;
}

/*SetPIDOthers: set antiWinup and rateLimit
 * Consider what happens when a controller starts at a zero state and
 * 	is commanded to start regulating at a high level. While the controller is driving hard toward the new level,
 * 	the system is still far away from the target setpoint, so the intg accumulates rapidly.
 * 	Eventually, even though the desired output level is reached, and passed,
 * 	the intg effects by themselves are enough to continue driving ahead at maximum.
 * 	This behavior is known as windup
 *
 * AntiWinup ~ Soft Integrator Anti-Windup --> reduces integrator changes rather than completely eliminating them. Select a reduction factor in the range 0.05 to 0.25.
 * HighLimit :  Since accumulation problems occur predominantly while the control output is at a limit, do not allow integrator adjustments during this time.
 * 	Defer updating the integrator state until after limits are checked.
 * RateLimit: This limits how fast the intg can change
 * */
void SetPIDOthers(PIDParams *pidParaInput, float antiWinup, float rateLimit,
		float delKp) {
	pidParaInput->antiWinup = antiWinup;
	pidParaInput->rateLimit = rateLimit;
	pidParaInput->delKp = delKp;
}
void SetDelT(PIDParams *pidParaInput, float delT) {
	pidParaInput->delT = delT;
}
void SetSetPoint(PIDParams *pidParaInput, float setPoint) {
	pidParaInput->setPoint = setPoint;
}

float PIDCompute3(PIDParams *pidParaInput, PID_state *pidStateInput,
		float currFeedback) {
	error = pidParaInput->setPoint - currFeedback;
//	Unbalanced Output Drive
	/*	It is not uncommon to find that the control loop works against a biased loading.
	 * For example, an actuator applies lift, and must work against gravity to move its load upward,
	 * but it must work with gravity to move the load downward. Ordinarily, PID action is the same in both directions,
	 * and this leads to pulling downward too hard while not pushing upward hard enough.
	 * */
	if (error <= 0.0) {
		error *= pidParaInput->delKp;
	} else {
		error /= pidParaInput->delKp;
	}
	kpFactor = pidParaInput->Kp * error;
//	float output = pidParaInput->Kp * error;

//Integrator Rate Limiting
	float ichange = error;
	if (ichange > pidParaInput->rateLimit) {
		ichange = pidParaInput->rateLimit;
	} else if (ichange < -pidParaInput->rateLimit) {
		ichange = -pidParaInput->rateLimit;
	}
	kiFactor = pidParaInput->Ki * pidStateInput->intg * pidParaInput->delT;
//	output += pidParaInput->Ki * pidStateInput->intg * pidParaInput->delT;

//	Integrator Latching n Soft Integrator Anti-Windup

	output = kiFactor + kpFactor;
	if (output > pidParaInput->highLimit) {
//		pidStateInput->intg += pidParaInput->antiWinup * ichange;
		output = pidParaInput->highLimit;
	} else if (output <= pidParaInput->lowLimit) {
//		pidStateInput->intg += pidParaInput->antiWinup * ichange;
		output = pidParaInput->lowLimit;
	} else {
		pidStateInput->intg += ichange;
	}

//	Removing Command Glitches from Derivative Response
	/*	For computing the derivative estimate, the current and previous setpoint errors are subtracted.
	 * When regulating a constant setpoint, this difference is exactly the same as subtracting the current
	 * and previous feedback values. When the setpoint is changed, however, the change in the setpoint
	 * looks like an instantaneous, "near-infinite" spike that hits the derivative gain hard.
	 * */
	kdFactor = (currFeedback - pidStateInput->deriv) / pidParaInput->delT
			* pidParaInput->Kd;
	output += kdFactor;
//	output += (currFeedback - pidStateInput->deriv) / pidParaInput->delT
//			* pidParaInput->Kd;
	pidStateInput->deriv = currFeedback;
//	output += pidParaInput->Kd * (error - pidStateInput->deriv);
//	pidStateInput->deriv = error;

//	Improving Derivative Response
//	Gain Adjustments
	return (output);
}

float PIDCompute(PIDParams *pidParaInput, PID_state *pidStateInput,
		float currFeedback) {
	return PIDCompute3(pidParaInput, pidStateInput, currFeedback);
}
