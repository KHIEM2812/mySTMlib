#include "MotorController.h"
#include <string.h>
#include "systemTimer.h"
#include "utility.h"
#include "myLib/Assert.h"

void MotorDataStructInitialize(motor_data *me) {
	memset(me, 0, sizeof(motor_data));
}
void MotorDataConfig0(motor_data *me) {
	me->motorCountsPerRev = MOTOR1_COUNTS_PERREV;
	me->motorPositionDeadband = POS_DEATHBAND;
	me->motorVelocityZeroDeathBand = MOTOR1_VELOCITY_ZERO;
	me->motorAccelerationLimit = ACCELERATIONLIMIT;
	me->motorPwmChangeLimit = PWMCHANGELIMIT;

	//other value set to 0
	me->motorCurrentSpeed = 0;
	me->motorCurrentPosition = 0;
	me->motorPrevPosition = 0;
	me->timeOfLastRecodedEncoder = 0;

	me->motorRequestedSpeed = 0;
	me->motorVelocitySetpoint = 0;
	me->motorVelocityRampedSetpoint = 0;
	me->motorPositionCommand = 0;
	me->motorPositionSetpoint = 0;
	me->motorDistancetoTarget = 0;
	me->motorIncrementalPWM = 0;
	me->motorOutputPWM = 0;
	me->motorOutputPWMWithLimit = 0;
}

void MotorControlleInitialize(MotorController* me) {
	memset(me, 0, sizeof(MotorController));
}
int IsMotorControllerEnable(MotorController* me) {
	return me->enableFlag;
}
void SetModeMotor(MotorController* me, uint16_t mode) {
	me->motorMode = mode;
}
void SetMotorEncoder(MotorController* me, encoder_t* setEncoder) {
	me->motorEncoder = setEncoder;
}
void SetMotorPort(MotorController* me, MotorPort* setMotorPort) {
	me->motorPort = setMotorPort;
}
void SetMotorData(MotorController* me, motor_data* setMotorData) {
	me->motorData = setMotorData;
}
void SetPIDVelocityMotor(MotorController* me, pidStruct_t* pid) {
	me->motorVelocityPID = pid;
}
void SetPIDPositionMotor(MotorController* me, pidStruct_t* pid) {
	me->motorPositionPID = pid;
}

void SetTargetMotorSpeed(MotorController* me, float velocitySetpoint) {
	if (IsMotorControllerEnable(me)) {
		me->motorData->motorRequestedSpeed = velocitySetpoint;
	}
}
void SetTargetMotorPosition(MotorController* me, int setPositionSetpoint) {
	if (IsMotorControllerEnable(me)) {
		me->motorData->motorPositionSetpoint = setPositionSetpoint;
	}
}
float GetCurrentMotorSpeed(MotorController* me) {
	if (IsMotorControllerEnable(me)) {
		return me->motorData->motorCurrentSpeed;
	} else
		return 0;
}
int GetCurrentMotorPosition(MotorController* me) {
	if (IsMotorControllerEnable(me)) {
		return me->motorData->motorCurrentPosition;
	} else
		return 0;
}

inline int IsMotorControllerStop(MotorController* me) {
	if (me->motorData->motorCurrentSpeed != 0) {
		return 0;
	} else
		return 1;
}
void StopMotorController(MotorController* me) {
	SetSoftStop(me->motorPort);
}
void ResetMotorController(MotorController* me);

void CalculateMotorSpeed(MotorController* me, unsigned int currentTime) {
	int deltaCounts;
	motor_data * meme = me->motorData;
	unsigned int deltaTicks = abs(currentTime - meme->timeOfLastRecodedEncoder);
	meme->motorCurrentPosition = GetEncoderCounts(me->motorEncoder); //will help to update EncoderCounter as well
	deltaCounts = meme->motorCurrentPosition - meme->motorPrevPosition;
	meme->motorPrevPosition = meme->motorCurrentPosition;
	meme->motorCurrentSpeed += (0.1
			* (((float) deltaCounts * 60.0 * SYSTEMTIMER_FREQ)
					/ (meme->motorCountsPerRev * (float) deltaTicks * 1.0)
					- meme->motorCurrentSpeed));

}

void UpdateMotorEncoderNSpeed(MotorController* me) {
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = abs(
			currentTime - me->motorData->timeOfLastRecodedEncoder);
	if (deltaTime > MOTOR_DATA_UPDATED_TICKS) {
		CalculateMotorSpeed(me, currentTime);
		me->motorData->timeOfLastRecodedEncoder = currentTime;
	}
}

float VelocityRamp(float vSetpoint, float vRampedSetpoint, float Alimit) {
	if (vSetpoint > vRampedSetpoint) {
		vRampedSetpoint += (Alimit / MOTORCONTROLLOOP_FREQ);

		if (vRampedSetpoint > vSetpoint)
			vRampedSetpoint = vSetpoint;
	} else if (vSetpoint < vRampedSetpoint) {
		vRampedSetpoint += -(Alimit / MOTORCONTROLLOOP_FREQ);

		if (vRampedSetpoint < vSetpoint)
			vRampedSetpoint = vSetpoint;
	}
	return vRampedSetpoint;
}
void VPIDRoutine(MotorController*me) {
	int setStop;
	motor_data * myMotorData = me->motorData;
	myMotorData->motorVelocitySetpoint = myMotorData->motorRequestedSpeed;
	myMotorData->motorVelocityRampedSetpoint = VelocityRamp(
			myMotorData->motorVelocitySetpoint,
			myMotorData->motorVelocityRampedSetpoint,
			myMotorData->motorAccelerationLimit);
	myMotorData->motorIncrementalPWM = PIDUpdate(me->motorVelocityPID,
			myMotorData->motorVelocityRampedSetpoint,
			myMotorData->motorCurrentSpeed);
	myMotorData->motorOutputPWM += myMotorData->motorIncrementalPWM;
	myMotorData->motorOutputPWMWithLimit = LimitToRange(
			myMotorData->motorOutputPWM, -myMotorData->motorPwmChangeLimit,
			myMotorData->motorPwmChangeLimit);

	setStop = 0;
	if ((myMotorData->motorVelocityRampedSetpoint == 0)
			&& (myMotorData->motorCurrentSpeed
					<= myMotorData->motorVelocityZeroDeathBand))
		setStop = 1;

	if (setStop == 1) {
		SetSoftStop(me->motorPort);
		myMotorData->motorPositionCommand = 0;
		myMotorData->motorVelocitySetpoint = 0;
		myMotorData->motorVelocityRampedSetpoint = 0;
		myMotorData->motorIncrementalPWM = 0;
		myMotorData->motorOutputPWM = 0;
		myMotorData->motorOutputPWMWithLimit = 0;
	} else {
		SetRotate(me->motorPort, (int) myMotorData->motorOutputPWMWithLimit);
	}
}
void PPIDRoutine(MotorController*me) {
	int setStop;
	motor_data * myMotorData = me->motorData;
	myMotorData->motorCurrentPosition = GetEncoderCounts(me->motorEncoder);
	//calculate distance error
	myMotorData->motorDistancetoTarget = abs(
			myMotorData->motorPositionSetpoint
					- myMotorData->motorCurrentPosition);
	//PID for posCmd
	myMotorData->motorPositionCommand = PIDUpdate(me->motorPositionPID,
			(float) myMotorData->motorPositionSetpoint,
			(float) myMotorData->motorCurrentPosition);
	if (myMotorData->motorPositionCommand == 0) {
		UartPrint("%d;%d\r\n", myMotorData->motorCurrentPosition,
				myMotorData->motorPositionSetpoint);
	}
	myMotorData->motorVelocitySetpoint = myMotorData->motorPositionCommand;
	//ramp the velocity increase
	myMotorData->motorVelocityRampedSetpoint = VelocityRamp(
			myMotorData->motorVelocitySetpoint,
			myMotorData->motorVelocityRampedSetpoint,
			myMotorData->motorAccelerationLimit);
	//PID for velocityCmd
	myMotorData->motorIncrementalPWM = PIDUpdate(me->motorVelocityPID,
			myMotorData->motorVelocityRampedSetpoint,
			myMotorData->motorCurrentSpeed);
	myMotorData->motorOutputPWM += myMotorData->motorIncrementalPWM;
	myMotorData->motorOutputPWMWithLimit = LimitToRange(
			myMotorData->motorOutputPWM, -myMotorData->motorPwmChangeLimit,
			myMotorData->motorPwmChangeLimit);

	//consider Stop????
	setStop = 0;
	if (myMotorData->motorDistancetoTarget < myMotorData->motorPositionDeadband)
		setStop = 1;
	if ((myMotorData->motorVelocityRampedSetpoint == 0)
			&& (myMotorData->motorCurrentSpeed
					<= myMotorData->motorVelocityZeroDeathBand))
		setStop = 1;

	if (setStop == 1) {
		SetSoftStop(me->motorPort);
		myMotorData->motorPositionCommand = 0;
		myMotorData->motorVelocitySetpoint = 0;
		myMotorData->motorVelocityRampedSetpoint = 0;
		myMotorData->motorIncrementalPWM = 0;
		myMotorData->motorOutputPWM = 0;
		myMotorData->motorOutputPWMWithLimit = 0;

	} else {
		SetRotate(me->motorPort, (int) myMotorData->motorOutputPWMWithLimit);
	}
}
void UpdateVOpenLoop(MotorController*me) {
	SetRotate(me->motorPort, me->motorData->motorRequestedSpeed);
}
void UpdateMotorControlLoop(MotorController* me) {
	unsigned int currentTime = GetSystemTimer();
	unsigned int deltaTime = abs(
			currentTime - me->motorData->timeOfLastPIDLoop);
	if (deltaTime > MOTORCONTROLLOOP_TICKS) {
		if (IsMotorControllerEnable(me)) {
			switch (me->motorMode) {
			case OPENED_LOOP_VELOCITY:
				UpdateVOpenLoop(me);
				break;
			case CLOSED_LOOP_VELOCITY:
				VPIDRoutine(me);
				break;
			case CLOSED_LOOP_POSITION:
				PPIDRoutine(me);
				break;
			}
		}
		me->motorData->timeOfLastPIDLoop = currentTime;
	}

}

void MotorControllerConfig0(MotorController* me) {
	me->enableFlag = 1; //the flag helps to make sure the pointer points to the legit region
	me->motorMode = 0;
	//init motorData
	motor_data* motorData = malloc(sizeof(motor_data));
	REQUIRE(motorData!=0);
	MotorDataStructInitialize(motorData);
	MotorDataConfig0(motorData);
	SetMotorData(me, motorData);

	//init motorPort
	MotorPort* motorPort = malloc(sizeof(MotorPort));
	REQUIRE(motorPort!=0);
	MotorPortStructInit(motorPort);
	MotorPort0Config(motorPort);
	SetMotorPort(me, motorPort);

	//init motorEncoder
	encoder_t* motorEncoder = malloc(sizeof(encoder_t));
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
			MOTOR1_VELOCITY_MAX_O, 0, 0, 0, 0);
	SetPIDVelocityMotor(me, veloPID);

	pidStruct_t* posPID = malloc(sizeof(pidStruct_t));
	REQUIRE(posPID!=0);
	PIDStructInit(posPID);
	PIDConfig(posPID, MOTOR1_POSITION_P, MOTOR1_POSITION_I, MOTOR1_POSITION_D,
			MOTOR1_POSITION_F, MOTOR1_POSITION_MAX_P, MOTOR1_POSITION_MAX_I,
			MOTOR1_POSITION_MAX_D, MOTOR1_POSITION_MAX_O,
			MOTOR1_POSITION_I_STATE_MAX, 0, 0, 0, 0);
	SetPIDPositionMotor(me, posPID);

}

void MotorControllerConfigForMAT(MotorController* me) {
	me->enableFlag = 1; //the flag helps to make sure the pointer points to the legit region
	me->motorMode = 0;
	//init motorData
	motor_data* motorData = malloc(sizeof(motor_data));
	REQUIRE(motorData!=0);
	MotorDataStructInitialize(motorData);
	MotorDataConfig0(motorData);
	SetMotorData(me, motorData);

	//init motorPort
	MotorPort* motorPort = malloc(sizeof(MotorPort));
	REQUIRE(motorPort!=0);
	MotorPortStructInit(motorPort);
	MotorPortConfigForMAT(motorPort);
	SetMotorPort(me, motorPort);

	//init motorEncoder
	encoder_t* motorEncoder = malloc(sizeof(encoder_t));
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
			MOTOR1_VELOCITY_MAX_O, 0, 0, 0, 0);
	SetPIDVelocityMotor(me, veloPID);

	pidStruct_t* posPID = malloc(sizeof(pidStruct_t));
	REQUIRE(posPID!=0);
	PIDStructInit(posPID);
	PIDConfig(posPID, MOTOR1_POSITION_P, MOTOR1_POSITION_I, MOTOR1_POSITION_D,
			MOTOR1_POSITION_F, MOTOR1_POSITION_MAX_P, MOTOR1_POSITION_MAX_I,
			MOTOR1_POSITION_MAX_D, MOTOR1_POSITION_MAX_O,
			MOTOR1_POSITION_I_STATE_MAX, 0, 0, 0, 0);
	SetPIDPositionMotor(me, posPID);
}
