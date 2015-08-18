/* Khiem
 * Dec 2, 2014
 * project:servo motor
 * description: is to make use motorPort interface, process motor_t data, control PID loop
 *
 */

#ifndef _motorController_h
#define _motorController_h

#include "MotorPort.h"
#include "Encoder.h"
#include "pid.h"

#define SECONDSPERMINUTE	(60.0)
typedef enum MotorModeEnum {
	OPENED_LOOP_VELOCITY = 0,
	CLOSED_LOOP_VELOCITY,
	OPENED_LOOP_POSITION,
	CLOSED_LOOP_POSITION
} MotorModeEnum;

typedef enum DirectionEnum {
	FORWARD = 0, REVERSE = 1
} DirectionEnum;

typedef struct motor_data {
	float motorCurrentSpeed;
	int motorCurrentPosition;
	int motorPrevPosition;
	unsigned int timeOfLastRecodedEncoder; //for cal speed
	unsigned int timeOfLastPIDLoop;
	unsigned int motorControlLoopTicks;

	float motorRequestedSpeed; //feed into motorVelocitySetpoint, in open loop mode

	float motorAccelerationLimit; //used in close loop
	float motorVelocityLimit;
	int motorPositionDeadband;
	float motorVelocityZeroDeathBand;
	float motorCountsPerRev;

	int motorPositionSetpoint;
	float motorVelocitySetpoint; //using with motorVelocityRampedSetpoint or motorAccelerationLimit to ramp the change of velocity
	float motorVelocityRampedSetpoint; //used in open loop

	float motorIncrementalPWM; //resulted in the velocity PID loop
	float motorOutputPWM; //convert to abs base
	float motorOutputPWMWithLimit;
	float motorPwmChangeLimit;
} MotorData;

typedef struct MotorController {
	int enableFlag;
	int motorMode;
	int id;
	MotorPort* motorPort;
	Encoder * motorEncoder; //it s attached with the motor
	MotorData* motorData;
	pidStruct_t* motorVelocityPID;
	pidStruct_t* motorPositionPID;
} MotorController;

//MotorPort interface
int IsMotorEnable(MotorPort * me);
void EnableMotor(MotorPort * me);
void DisableMotor(MotorPort * me);
void SetRotate(MotorPort * me, int iValue); //Port--> setPWM
void SetStop(MotorPort * me); //Port
void SetSoftStop(MotorPort * me);

void MotorDataStructInitialize(MotorData *me);
void MotorDataConfig0(MotorData *me);

void MotorControlleInitialize(MotorController* me);
void MotorControllerConfig0(MotorController* me);
void MotorControllerConfigForMAT(MotorController* me);

int IsMotorControllerEnable(MotorController* me);

void SetModeMotor(MotorController* me, uint16_t mode);
void SetMotorEncoder(MotorController* me, Encoder* setEncoder);
void SetMotorPort(MotorController* me, MotorPort* setMotorPort);
void SetPIDVelocityMotor(MotorController* me, pidStruct_t* pid);
void SetPIDPositionMotor(MotorController* me, pidStruct_t* pid);
void SetTargetMotorVelocity(MotorController* me, float velocitySetpoint);
void SetTargetMotorPosition(MotorController* me, int setPositionSetpoint);
float GetCurrentMotorSpeed(MotorController* me);
int GetCurrentMotorPosition(MotorController* me);

int IsMotorControllerStop(MotorController* me);
void StopMotorController(MotorController* me);
void ResetMotorController(MotorController* me);
void ResetSetpointsInMotorData(MotorController* me1);
void ResetMotorDataExcludeSetpoints(MotorController* me1);

void UpdateMotorEncoderNSpeed(MotorController* me);
void UpdateMotorControlLoop(MotorController* me);

float RampUp(float vSetpoint, float vRampedSetpoint, float vInc);

#endif
