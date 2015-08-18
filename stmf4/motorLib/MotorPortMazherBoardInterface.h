#ifndef _MotorPortMazherBoardInterface_h
#define _MotorPortMazherBoardInterface_h

#include "motorLib/Encoder.h"
#include "motorLib/MotorPort.h"
#include "motorLib/MotorController.h"
#include "MotorPortMazherBoardInterface.h"
#include "cAbsEncoder.h"

void SetRotateMotorMazherBoardInterface(MotorPort * me, int iValue);
void EncoderPortPA67ConfigMazherBoardInterface(Encoder* me);
void MotorOnePortConfigMazherBoardInterface(MotorPort * me);
void EncoderPortPB67ConfigMazherBoardInterface(Encoder* me);
void MotorTwoPortConfigMazherBoardInterface(MotorPort * me);
void EnableMotorMazherBoardInterface(MotorPort* me);
void DisableMotorMazherBoardInterface(MotorPort* me);
int IsEnableMotorMazherBoardInterface(MotorPort* me);
void SetSoftStopMotorMazherBoardInterface(MotorPort * me);

void MotorOneControllerConfigMazherBoardInterface(MotorController* me);
void MotorTwoControllerConfigMazherBoardInterface(MotorController* me);
void AbsEncoderPortConfigMazherBoardInterface();
int GetAbsEncoderCountsMazherBoardInterface(cAbsEncoder* me);
#endif
