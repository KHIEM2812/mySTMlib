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
	int motorCurrentPosition; //TODO: initialize n config
	int motorPrevPosition;
	unsigned int timeOfLastRecodedEncoder; //for cal speed
	unsigned int timeOfLastPIDLoop;

	float motorRequestedSpeed; //feed into motorVelocitySetpoint, in open loop mode

	float motorAccelerationLimit; //used in close loop
	u32 motorPositionDeadband;
	float motorVelocityZeroDeathBand;
	float motorCountsPerRev;

	float motorVelocitySetpoint; //using with motorVelocityRampedSetpoint or motorAccelerationLimit to ramp the change of velocity
	float motorVelocityRampedSetpoint; //used in open loop
	float motorPositionCommand;
	int motorPositionSetpoint;
	int motorDistancetoTarget;

	float motorIncrementalPWM; //resulted in the velocity PID loop
	float motorOutputPWM; //convert to abs base
	float motorOutputPWMWithLimit;
	float motorPwmChangeLimit;
} motor_data;

typedef struct MotorController {
	int enableFlag;
	int motorMode;
	MotorPort* motorPort;
	encoder_t * motorEncoder; //it s attached with the motor
	motor_data* motorData;
	pidStruct_t* motorVelocityPID;
	pidStruct_t* motorPositionPID;
} MotorController;

void MotorDataStructInitialize(motor_data *me);
void MotorDataConfig0(motor_data *me);

void MotorControlleInitialize(MotorController* me);
void MotorControllerConfig0(MotorController* me);
void MotorControllerConfigForMAT(MotorController* me);

int IsMotorControllerEnable(MotorController* me);

void SetModeMotor(MotorController* me, uint16_t mode);
void SetMotorEncoder(MotorController* me, encoder_t* setEncoder);
void SetMotorPort(MotorController* me, MotorPort* setMotorPort);
void SetMotorData(MotorController* me, motor_data* setMotorData);
void SetPIDVelocityMotor(MotorController* me, pidStruct_t* pid);
void SetPIDPositionMotor(MotorController* me, pidStruct_t* pid);
void SetTargetMotorSpeed(MotorController* me, float velocitySetpoint);
void SetTargetMotorPosition(MotorController* me, int setPositionSetpoint);
float GetCurrentMotorSpeed(MotorController* me);
int GetCurrentMotorPosition(MotorController* me);

int IsMotorControllerStop(MotorController* me);
void StopMotorController(MotorController* me);
void ResetMotorController(MotorController* me);

void UpdateMotorEncoderNSpeed(MotorController* me);
void UpdateMotorControlLoop(MotorController* me);
#endif
