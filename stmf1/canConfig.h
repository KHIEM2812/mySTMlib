#ifndef CANCONFIG_H
#define CANCONFIG_H

#include "STM32_Nucleo/stm32f1xx/stm32f1xx_nucleo.h"
#include <stdio.h>
#include "utility.h"
#include "usart.h"
#include "IBox.h"
#include "config.h"

#define RXBUFFER_SIZE 				3	//to avoid OVERWRITE btw main Loop and ISR
#define CAN 1

#if CAN == 1
#define CANx							CAN1
#define CANx_CLK						RCC_APB1Periph_CAN1
#define CANx_CLK_INIT					RCC_APB1PeriphClockCmd
#define CANx_TX_IRQn					xxx
#define CANx_RX0_IRQn					USB_LP_CAN1_RX0_IRQn
#define CANx_RX0_IRQHandler				USB_LP_CAN1_RX0_IRQHandler
#define CANx_TX_IRQHandler	 			xxx

#define CANx_TX_PIN						GPIO_Pin_9
#define CANx_TX_GPIO_PORT				GPIOB
#define CANx_TX_GPIO_CLK				RCC_APB2Periph_GPIOB
#define CANx_TX_AF                     	GPIO_AF_CAN1

#define CANx_RX_PIN						GPIO_Pin_8
#define CANx_RX_GPIO_PORT				GPIOB
#define CANx_RX_GPIO_CLK				RCC_APB2Periph_GPIOB

#endif

//									PRI		MSG_ID		NODE_ID
//Standard 11 bit CAN identifer 	XX		XXXX		XXXXX

#define CAN_PRIORITY_OFFSET 		9
#define CAN_MSGID_OFFSET			5
#define CAN_NODEID_OFFSET			0

//Transmission priorities
#define CAN_PRIORITY_URGENT			(0  << CAN_PRIORITY_OFFSET)
#define CAN_PRIORITY_HIGH			(1  << CAN_PRIORITY_OFFSET)
#define CAN_PRIORITY_MEDIUM			(2  << CAN_PRIORITY_OFFSET)
#define CAN_PRIORITY_LOW			(3  << CAN_PRIORITY_OFFSET)
#define CAN_PRIORTIY_MASK			(0b11 << CAN_PRIORITY_OFFSET)

//Message IDs
#define CAN_MSG_EMERGENCY_STOP		(0 << CAN_MSGID_OFFSET)
#define CAN_MSG_VELOCITY			(1 << CAN_MSGID_OFFSET)
#define CAN_MSG_HEARTBEAT			(2 << CAN_MSGID_OFFSET)
#define CAN_MSG_POS_REQ				(3 << CAN_MSGID_OFFSET)
#define CAN_MSG_MOTOR_ENABLE		(4 << CAN_MSGID_OFFSET)
#define CAN_MSG_NACK				(15 << CAN_MSGID_OFFSET)
#define CAN_MSG_ACK					(16 << CAN_MSGID_OFFSET)
#define CAN_MSGID_MASK				(0b1111 << CAN_MSGID_OFFSET)

#define CAN_32BIT_FILTER_OFFSET		3
#define CAN_16BIT_FILTER_OFFSET		5

#define CAN_RTR_32BIT_OFFSET		1
#define CAN_RTR_16BIT_OFFSET		4

#define CAN_RTR_32BIT_ENABLE		(1 << CAN_RTR_32BIT_OFFSET)

#define STD_TO_EXTENDED_OFFSET		18

// 0 -31 node IDs
#define CAN_NODEID					(DEVICE_ID << CAN_NODEID_OFFSET)
#define CAN_NODEID_MASK				(0b11111 << CAN_NODEID_OFFSET)

#define EMERGENCY_STOP_FILTER		0
#define VELOCITY_FILTER				1
#define HEARTBEAT_REQ_FILTER		2
#define POS_REQ_FILTER				3
#define MOTOR_ENABLE_FILTER			4

typedef enum {
	NoCmd = 0,
	BoxInfoRQ,
	WeightRQ,
	BatteryRQ,
	Save2Flash,
	setNOWEIGHTLV,
	setEMPTYWEIGHTLV,
	setFULLWEIGHTLV,
	setWEIGHTTOLLERANCE,
	setCONFIG_VERSION,
	getNOWEIGHTLV,
	getEMPTYWEIGHTLV,
	getFULLWEIGHTLV,
	getWEIGHTTOLLERANCE,
	getCONFIG_VERSION

} canCmd;

void CANConfig();
uint8_t SendMsg(uint16_t stdid, uint8_t dlc, uint8_t* data);
uint8_t SendMsg1(void);
uint8_t SendBatteryState(void);
uint8_t SendPepperState(void);
uint8_t SendBoxState(void);

void USB_LP_CAN1_RX0_IRQHandler(void);
CanRxMsg *RetrieveFromQueue(void);

void HandleCANRPDO(void);
void HandleCANTPDO(void);

#endif
