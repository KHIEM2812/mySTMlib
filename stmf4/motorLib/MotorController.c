#include "MotorController.h"
#include <string.h>
#include "systemTimer.h"
#include "utility.h"
#include "myLib/Assert.h"
#include "cAlienCatherPeripheral.h"
#include <math.h>
void MotorDataStructInitialize(MotorData *me) {
	memset(me, 0, sizeof(MotorData));
}
void MotorDataConfig0(MotorData *me) {
	me->motorCountsPerRev = MOTOR1_COUNTS_PERREV;
	me->motorPositionDeadband = MOTOR1_POS_DEATHBAND;
	me->motorVelocityZeroDeathBand = MOTOR1_VELOCITY_ZERO;
	me->motorAccelerationLimit = MOTOR1_ACCELERATION_LIMIT;
	me->motorVelocityLimit = MOTOR1_VELOCITY_RPM_LIMIT;
	me->motorPwmChangeLimit = MOTOR1_PWMCHANGELIMIT;
}

int IsMotorEnable(MotorPort * me) {
	return IsEnableMotorMazherBoardInterface(me);
}
void EnableMotor(MotorPort * me) {
	EnableMotorMazherBoardInterface(me);
}
void DisableMotor(MotorPort * me) {
	DisableMotorMazherBoardInterface(me);
}
void SetRotate(MotorPort * me, int iValue) {
	SetRotateMotorMazherBoardInterface(me, iValue);
}
void SetSoftStop(MotorPort * me) {
	SetSoftStopMotorMazherBoardInterface(me);
}

void MotorControlleInitialize(MotorController* me) {
	memset(me, 0, sizeof(MotorController));
}
int IsMotorControllerEnable(MotorController* me) {
	return IsMotorEnable(me->motorPort);
}
void SetModeMotor(MotorController* me, uint16_t mode) {
	me->motorMode = mode;
}
void SetMotorEncoder(MotorController* me, Encoder* setEncoder) {
	me->motorEncoder = setEncoder;
}
void SetMotorPort(MotorController* me, MotorPort* setMotorPort) {
	me->motorPort = setMotorPort;
}

void SetPIDVelocityMotor(MotorController* me, pidStruct_t* pid) {
	me->motorVelocityPID = pid;
}
void SetPIDPositionMotor(MotorController* me, pidStruct_t* pid) {
	me->motorPositionPID = pid;
}

void SetTargetMotorVelocity(MotorController* me, float velocitySetpoint) {
	if (me->motorMode == CLOSED_LOOP_VELOCITY) {
		me->motorData->motorRequestedSpeed = velocitySetpoint;
	}
}
void SetTargetMotorPosition(MotorController* me, int setPositionSetpoint) {
	if (me->motorMode == CLOSED_LOOP_POSITION) {
		me->motorData->motorPositionSetpoint = setPositionSetpoint;
	}
}
float GetCurrentMotorSpeed(MotorController* me) {
	return me->motorData->motorCurrentSpeed;
}
int GetCurrentMotorPosition(MotorController* me) {
	return me->motorData->motorCurrentPosition;
}

inline int IsMotorControllerStop(MotorController* me) {
	if (me->motorData->motorCurrentSpeed != 0) {
		return 0;
	} else
		return 1;
}
void StopMotorController(MotorController* me) {
	ResetSetpointsInMotorData(me);
	ResetMotorDataExcludeSetpoints(me); //this ll give a sharp stop
	me->motorData->motorCurrentSpeed = 0;
	SetSoftStop(me->motorPort);
}
void ResetSetpointsInMotorData(MotorController* me1) {
	MotorData* me = me1->motorData;
	me->motorRequestedSpeed = 0;
	me->motorPositionSetpoint = 0;
}
void ResetMotorDataExcludeSetpoints(MotorController* me1) {
	MotorData* me = me1->motorData;
	me->motorVelocitySetpoint = 0;
	me->motorVelocityRampedSetpoint = 0;
	me->motorIncrementalPWM = 0;
	me->motorOutputPWM = 0;
	me->motorOutputPWMWithLimit = 0;
}
void CalculateMotorSpeed(MotorController* me, unsigned int currentTime) {
	int deltaCounts;
	MotorData * meme = me->motorData;
	unsigned int deltaTicks = currentTime - meme->timeOfLastRecodedEncoder;
	meme->motorCurrentPosition = GetEncoderCounts(me->motorEncoder); //will help to update EncoderCounter as well
	deltaCounts = meme->motorCurrentPosition - meme->motorPrevPosition;
	meme->motorPrevPosition = meme->motorCurrentPosition;
	//float rawSpeed = deltaCounts*encCONST/deltaTicks;
	float rawSpeed = (deltaCounts * SECONDSPERMINUTE * SYSTEMTIMER_FREQ)
			/ (meme->motorCountsPerRev * deltaTicks);
	meme->motorCurrentSpeed += (0.5 * (rawSpeed - meme->motorCurrentSpeed)); //low pass filter

}

void UpdateMotorEncoderNSpeed(MotorController* me) {
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = currentTime
			- me->motorData->timeOfLastRecodedEncoder;
	if (deltaTime > MOTOR_DATA_UPDATED_TICKS) {
		CalculateMotorSpeed(me, currentTime);
		me->motorData->timeOfLastRecodedEncoder = currentTime;
	}
}

float RampUp(float vSetpoint, float vRampedSetpoint, float vInc) {
	if (vSetpoint > vRampedSetpoint) {
		vRampedSetpoint += (vInc);
		if (vRampedSetpoint > vSetpoint)
			vRampedSetpoint = vSetpoint;
	} else if (vSetpoint < vRampedSetpoint) {
		vRampedSetpoint -= (vInc);
		if (vRampedSetpoint < vSetpoint)
			vRampedSetpoint = vSetpoint;
	}
	return vRampedSetpoint;
}
void PIDRoutine(MotorController*me) {
	MotorData * myMotorData = me->motorData;
	if (me->motorMode == OPENED_LOOP_VELOCITY) {
		myMotorData->motorOutputPWMWithLimit =
				me->motorData->motorRequestedSpeed;
	} else {
		if (me->motorMode == CLOSED_LOOP_POSITION) {
			//TODO
			myMotorData->motorCurrentPosition = GetEncoderCounts(
					me->motorEncoder);
			myMotorData->motorVelocitySetpoint = PIDUpdate(me->motorPositionPID,
					(float) myMotorData->motorPositionSetpoint,
					(float) myMotorData->motorCurrentPosition);
		} else if (me->motorMode == CLOSED_LOOP_VELOCITY) {
			myMotorData->motorPositionSetpoint = 0;
			myMotorData->motorVelocitySetpoint =
					myMotorData->motorRequestedSpeed;
		}

		myMotorData->motorVelocitySetpoint = LimitToRange(
				myMotorData->motorVelocitySetpoint,
				-myMotorData->motorVelocityLimit,
				myMotorData->motorVelocityLimit);
		//ramp the velocity increase
		myMotorData->motorVelocityRampedSetpoint = RampUp(
				myMotorData->motorVelocitySetpoint,
				myMotorData->motorVelocityRampedSetpoint,
				myMotorData->motorAccelerationLimit * SYSTEMTIMER_FREQ
						/ myMotorData->motorControlLoopTicks);
		//PID for velocityCmd
		myMotorData->motorIncrementalPWM = PIDUpdate(me->motorVelocityPID,
				myMotorData->motorVelocityRampedSetpoint,
				myMotorData->motorCurrentSpeed);
		myMotorData->motorOutputPWM += myMotorData->motorIncrementalPWM;
		myMotorData->motorOutputPWMWithLimit = LimitToRange(
				myMotorData->motorOutputPWM, -myMotorData->motorPwmChangeLimit,
				myMotorData->motorPwmChangeLimit);

		//consider Stop????
		int setStop = 0;
		int errorDistance = abs(
				myMotorData->motorPositionSetpoint
						- myMotorData->motorCurrentPosition);
		int isInPosDeathbandCriteria = errorDistance
				< myMotorData->motorPositionDeadband;
		int isInVelDeathbandCriteria = fabsf(myMotorData->motorCurrentSpeed)
				< myMotorData->motorVelocityZeroDeathBand
				&& fabsf(myMotorData->motorVelocityRampedSetpoint)
						< myMotorData->motorVelocityZeroDeathBand;
		if (me->motorMode == CLOSED_LOOP_POSITION && (isInPosDeathbandCriteria))
			setStop = 1;
		if (me->motorMode == CLOSED_LOOP_VELOCITY && isInVelDeathbandCriteria)
			setStop = 1;

		if (setStop == 1) {
			ResetMotorDataExcludeSetpoints(me);
		}
	}

	SetRotate(me->motorPort, (int) myMotorData->motorOutputPWMWithLimit);
}
void UpdateVOpenLoop(MotorController*me) {
	SetRotate(me->motorPort, me->motorData->motorRequestedSpeed);
}
void UpdateMotorControlLoop(MotorController* me) {
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = currentTime - me->motorData->timeOfLastPIDLoop;
	if (deltaTime > me->motorData->motorControlLoopTicks) {
		if (IsMotorControllerEnable(me)) {
			switch (me->motorMode) {
			case OPENED_LOOP_VELOCITY:
				PIDRoutine(me);
				break;
			case CLOSED_LOOP_VELOCITY:
				PIDRoutine(me);
				break;
			case CLOSED_LOOP_POSITION:
				PIDRoutine(me);
				break;
			}
		}
		me->motorData->timeOfLastPIDLoop = currentTime;
	}
}

void MotorControllerConfig0(MotorController* me) {
	me->enableFlag = 1; //the flag helps to make sure the pointer points to the legit region
	me->motorMode = 0;
	me->id = 0;
	//init motorData
	MotorData* motorData = malloc(sizeof(MotorData));
	REQUIRE(motorData!=0);
	MotorDataStructInitialize(motorData);
	MotorDataConfig0(motorData);
	me->motorData = motorData;

	//init motorPort
	MotorPort* motorPort = malloc(sizeof(MotorPort));
	REQUIRE(motorPort!=0);
	MotorPortStructInit(motorPort);
	MotorPort0Config(motorPort);
	SetMotorPort(me, motorPort);

	//init motorEncoder
	Encoder* motorEncoder = malloc(sizeof(Encoder));
	REQUIRE(motorEncoder!=0);
	EncStructInit(motorEncoder);
	Enc0Config(motorEncoder);
	SetMotorEncoder(me, motorEncoder);

	//init pid
	pidStruct_t* veloPID = malloc(sizeof(pidStruct_t));
	REQUIRE(veloPID!=0);
	PIDStructInit(veloPID);
	PIDConfig(veloPID, MOTOR1_VELOCITY_P, MOTOR1_VELOCITY_I, MOTOR1_VELOCITY_D,
			MOTOR1_VELOCITY_F, MOTOR1_VELOCITY_MAX_P, MOTOR1_VELOCITY_MAX_I,
			MOTOR1_VELOCITY_I_STATE_MAX, MOTOR1_VELOCITY_MAX_D,
			MOTOR1_VELOCITY_MAX_O, MOTOR1_VELOCITY_DELKP, MOTOR1_VELOCITY_KZ);
	SetPIDVelocityMotor(me, veloPID);

	pidStruct_t* posPID = malloc(sizeof(pidStruct_t));
	REQUIRE(posPID!=0);
	PIDStructInit(posPID);
	PIDConfig(posPID, MOTOR1_POSITION_P, MOTOR1_POSITION_I, MOTOR1_POSITION_D,
			MOTOR1_POSITION_F, MOTOR1_POSITION_MAX_P, MOTOR1_POSITION_MAX_I,
			MOTOR1_POSITION_I_STATE_MAX, MOTOR1_POSITION_MAX_D,
			MOTOR1_POSITION_MAX_O, MOTOR1_POSITION_DELKP, MOTOR1_POSITION_KZ);
	SetPIDPositionMotor(me, posPID);

}
void MotorControllerConfigForMAT(MotorController* me) {
	me->enableFlag = 1; //the flag helps to make sure the pointer points to the legit region
	me->motorMode = 0;
	//init motorData
	MotorData* motorData = malloc(sizeof(MotorData));
	REQUIRE(motorData!=0);
	MotorDataStructInitialize(motorData);
	MotorDataConfig0(motorData);
	me->motorData = motorData;

	//init motorPort
	MotorPort* motorPort = malloc(sizeof(MotorPort));
	REQUIRE(motorPort!=0);
	MotorPortStructInit(motorPort);
	MotorPortConfigForMAT(motorPort);
	SetMotorPort(me, motorPort);

	//init motorEncoder
	Encoder* motorEncoder = malloc(sizeof(Encoder));
	REQUIRE(motorEncoder!=0);
	EncStructInit(motorEncoder);
	Enc0Config(motorEncoder);
	SetMotorEncoder(me, motorEncoder);

	//init pid
	pidStruct_t* veloPID = malloc(sizeof(pidStruct_t));
	REQUIRE(veloPID!=0);
	PIDStructInit(veloPID);
	PIDConfig(veloPID, MOTOR1_VELOCITY_P, MOTOR1_VELOCITY_I, MOTOR1_VELOCITY_D,
			MOTOR1_VELOCITY_F, MOTOR1_VELOCITY_MAX_P, MOTOR1_VELOCITY_MAX_I,
			MOTOR1_VELOCITY_I_STATE_MAX, MOTOR1_VELOCITY_MAX_D,
			MOTOR1_VELOCITY_MAX_O, MOTOR1_VELOCITY_DELKP, MOTOR1_VELOCITY_KZ);
	SetPIDVelocityMotor(me, veloPID);

	pidStruct_t* posPID = malloc(sizeof(pidStruct_t));
	REQUIRE(posPID!=0);
	PIDStructInit(posPID);
	PIDConfig(posPID, MOTOR1_POSITION_P, MOTOR1_POSITION_I, MOTOR1_POSITION_D,
			MOTOR1_POSITION_F, MOTOR1_POSITION_MAX_P, MOTOR1_POSITION_MAX_I,
			MOTOR1_POSITION_I_STATE_MAX, MOTOR1_POSITION_MAX_D,
			MOTOR1_POSITION_MAX_O, MOTOR1_POSITION_DELKP, MOTOR1_POSITION_KZ);
	SetPIDPositionMotor(me, posPID);
}
