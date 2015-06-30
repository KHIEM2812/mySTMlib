#ifndef PIDLIB_H
#define PIDLIB_H

#include "STM32F4-Discovery/stm32f4_discovery.h"
//1 for Slow
//2 for Fast

typedef struct {
	// Gain parameters
	float Kp; // Loop gain parameter
	float Ki; // Integrator time constant
	float Kd; // Differentiator time constant
	float delT; // Update time interval
	float lowLimit;
	float highLimit;
	float antiWinup;
	float rateLimit;
	float delKp; //for Unbalanced Output Drive
	// Setpoint parameters
	float setPoint; // Regulated level to maintain
} PIDParams;

typedef struct {
	// Controller state
	float intg; // Summation of setpoint errors
	float deriv; // Previous setpoint error
} PID_state;

void SetTunings(PIDParams* pidParaInput, float Kp, float Ki, float Kd);
void SetLimits(PIDParams* pidParaInput, float lowLimit, float highLimit);
void SetDelT(PIDParams* pidParaInput, float delT);
void SetSetPoint(PIDParams* pidParaInput, float setPoint);
void SetPIDOthers(PIDParams* pidParaInput, float antiWinup, float rateLimit, float delKp);
void InitPIDState(PID_state * state);
void InitPIDPara(PIDParams * params);

void SetTunings1(float Kp, float Ki, float Kd);
void SetTunings2(float Kp, float Ki, float Kd);
void SetSetPoint1(float input);
void SetSetPoint2(float input);
float PIDCompute1(float iinput);
float PIDCompute2(float iinput);
float PIDCompute3(PIDParams* pidParaInput, PID_state* pidStateInput,
		float currFeedback);
float PIDCompute(PIDParams* pidParaInput, PID_state* pidStateInput,
		float currFeedback);
#endif
