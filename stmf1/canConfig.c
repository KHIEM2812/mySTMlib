#include "canConfig.h"

extern char myString[200];
extern IBoxStruct thisBox;
extern volatile uint16_t ADC_Results[ADC_CHANNELS];

CanTxMsg TxMessage;
CanRxMsg* RxMessage;
volatile CanRxMsg bufferRxMsg[RXBUFFER_SIZE];

uint32_t currentListenNode = 0xffffffff;
uint32_t receiveIndex = 0;
volatile uint32_t numberRPDO = 0;
volatile int start = 0, endf = 0, active = 0;

uint8_t busState = 0; //1=avail, 2 =not avail
char doesReadCANMsgFlag = 0;
u32 command = 0;

void CANConfig3(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(CANx_TX_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(CANx_RX_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(CANx_CLK, ENABLE);

	GPIO_DeInit(CANx_TX_GPIO_PORT);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = CANx_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = CANx_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(CANx_RX_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);

	CAN_DeInit(CANx);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN Baudrate = 250kBps*/
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 24; // change to 4 for 1MBps
	CAN_Init(CANx, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = CANx_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void CANConfig(void) {
	CANConfig3();
}

//CANFIFO1 pending msg interrupt: save new msg into the circular buffer
void CANx_RX0_IRQHandler(void) {
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
		numberRPDO++;
		CAN_Receive(CAN1, CAN_FIFO0, (CanRxMsg*) &bufferRxMsg[endf]);
		endf = (endf + 1) % RXBUFFER_SIZE;
		if (active < RXBUFFER_SIZE) {
			active++;
		} else {
			/* Overwriting the oldest. Move start to next-oldest */
			start = (start + 1) % RXBUFFER_SIZE;
		}
	}
}
//extract msg from the circular buffer of received msg
CanRxMsg *RetrieveFromQueue(void) {
	CanRxMsg *p;
	if (active) {
		p = (CanRxMsg*) &bufferRxMsg[start];
		start = (start + 1) % RXBUFFER_SIZE;
		active--;
	} else {
		p = NULL;
	}

	return p;
}

u32 DetermineCommand(CanRxMsg* RxMessageI) {
	u16 id = (u16) ((RxMessageI->Data[1] << 8) | (RxMessageI->Data[0]));
	u32 command = 0;
	if (RxMessageI->StdId == (u32) (0x400 + BoxInfoRQ) && id == thisBox.boxID) {
		command = BoxInfoRQ;
#ifdef DEBUG
		sprintf(myString, "BoxInfoRQ\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + WeightRQ)
			&& id == thisBox.boxID) {
		command = WeightRQ;
#ifdef DEBUG
		sprintf(myString, "WeightRQ\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + BatteryRQ)
			&& id == thisBox.boxID) {
		command = BatteryRQ;
#ifdef DEBUG
		sprintf(myString, "BatteryRQ\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + Save2Flash)
			&& id == thisBox.boxID) {
		command = Save2Flash;
#ifdef DEBUG
		sprintf(myString, "Save2Flash\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + setNOWEIGHTLV)
			&& id == thisBox.boxID) {
		command = setNOWEIGHTLV;
#ifdef DEBUG
		sprintf(myString, "setNOWEIGHTLV\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + setEMPTYWEIGHTLV)
			&& id == thisBox.boxID) {
		command = setEMPTYWEIGHTLV;
#ifdef DEBUG
		sprintf(myString, "setEMPTYWEIGHTLV\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + setFULLWEIGHTLV)
			&& id == thisBox.boxID) {
		command = setFULLWEIGHTLV;
#ifdef DEBUG
		sprintf(myString, "setFULLWEIGHTLV\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + setWEIGHTTOLLERANCE)
			&& id == thisBox.boxID) {
		command = setWEIGHTTOLLERANCE;
#ifdef DEBUG
		sprintf(myString, "setWEIGHTTOLLERANCE\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + setCONFIG_VERSION)
			&& id == thisBox.boxID) {
		command = setCONFIG_VERSION;
#ifdef DEBUG
		sprintf(myString, "setCONFIG_VERSION\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + getNOWEIGHTLV)
			&& id == thisBox.boxID) {
		command = getNOWEIGHTLV;
#ifdef DEBUG
		sprintf(myString, "getNOWEIGHTLV\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + getEMPTYWEIGHTLV)
			&& id == thisBox.boxID) {
		command = getEMPTYWEIGHTLV;
#ifdef DEBUG
		sprintf(myString, "getEMPTYWEIGHTLV\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + getFULLWEIGHTLV)
			&& id == thisBox.boxID) {
		command = getFULLWEIGHTLV;
#ifdef DEBUG
		sprintf(myString, "getFULLWEIGHTLV\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + getWEIGHTTOLLERANCE)
			&& id == thisBox.boxID) {
		command = getWEIGHTTOLLERANCE;
#ifdef DEBUG
		sprintf(myString, "getWEIGHTTOLLERANCE\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else if (RxMessageI->StdId == (u32) (0x400 + getCONFIG_VERSION)
			&& id == thisBox.boxID) {
		command = getCONFIG_VERSION;
#ifdef DEBUG
		sprintf(myString, "getCONFIG_VERSION\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	} else {
		command = NoCmd;
#ifdef DEBUG
		sprintf(myString, "NoCmd\r\n");
		serialPrint2(myString, strlen(myString) + 1);
#endif
	}
	return command;
}

u32 ConvertU8ToU32(u8* data, int index) {
	return (u32) ((data[index + 3] << 24) | (data[index + 2] << 16)
			| (data[index + 1] << 8) | (data[index]));
}

void float2Bytes(u8* bytes_temp, float float_variable) {
	union {
		float a;
		unsigned char bytes[4];
	} thing;
	thing.a = float_variable;
	memcpy(bytes_temp, thing.bytes, 4);
}

uint8_t SendMsg1(void) {
	TxMessage.StdId = 0x321;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 1;
	TxMessage.Data[0] = 2;
	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}

uint8_t SendBoxState(void) {
	TxMessage.StdId = 0x400 + BoxInfoRQ;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	TxMessage.Data[2] = (u8) thisBox.pepperSprayState1;
	TxMessage.Data[3] = (u8) (ADC_Results[0] & 0x00ff);
	TxMessage.Data[4] = (u8) ((ADC_Results[0] >> 8) & 0x00ff);

	TxMessage.Data[5] = (u8) thisBox.boxBatteryState1;
	TxMessage.Data[6] = (u8) (ADC_Results[1] & 0x00ff);
	TxMessage.Data[7] = (u8) ((ADC_Results[1] >> 8) & 0x00ff);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}

uint8_t SendPepperState(void) {
	TxMessage.StdId = 0x400 + WeightRQ;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	TxMessage.Data[2] = (u8) thisBox.pepperSprayState1;
	TxMessage.Data[3] = (u8) (ADC_Results[0] & 0x00ff);
	TxMessage.Data[4] = (u8) ((ADC_Results[0] >> 8) & 0x00ff);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}

uint8_t SendBatteryState(void) {
	TxMessage.StdId = 0x400 + BatteryRQ;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	TxMessage.Data[2] = (u8) thisBox.boxBatteryState1;
	TxMessage.Data[3] = (u8) (ADC_Results[1] & 0x00ff);
	TxMessage.Data[4] = (u8) ((ADC_Results[1] >> 8) & 0x00ff);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}

uint8_t SendNOWEIGHTLV(void) {
	TxMessage.StdId = 0x400 + getNOWEIGHTLV;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 6;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	float2Bytes(&(TxMessage.Data[2]), p[NOWEIGHTLV]);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}

uint8_t SendEMPTYWEIGHTLV(void) {
	TxMessage.StdId = 0x400 + getEMPTYWEIGHTLV;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 6;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	float2Bytes(&(TxMessage.Data[2]), p[EMPTYWEIGHTLV]);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}

uint8_t SendFULLWEIGHTLV(void) {
	TxMessage.StdId = 0x400 + getFULLWEIGHTLV;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 6;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	float2Bytes(&(TxMessage.Data[2]), p[FULLWEIGHTLV]);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}
uint8_t SendWEIGHTTOLLERANCE(void) {
	TxMessage.StdId = 0x400 + getWEIGHTTOLLERANCE;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 6;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	float2Bytes(&(TxMessage.Data[2]), p[WEIGHTTOLLERANCE]);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}
uint8_t SendCONFIG_VERSION(void) {
	TxMessage.StdId = 0x400 + getCONFIG_VERSION;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 6;
	TxMessage.Data[0] = (u8) (thisBox.boxID & 0x00ff);
	TxMessage.Data[1] = (u8) ((thisBox.boxID >> 8) & 0x00ff);

	float2Bytes(&(TxMessage.Data[2]), p[CONFIG_VERSION]);

	uint8_t mailbox = CAN_Transmit(CANx, &TxMessage);
	if (mailbox == CAN_TxStatus_NoMailBox) {
		//send too fast!!!
	}
	uint8_t status = CAN_TransmitStatus(CANx, mailbox);
	return status;
}

void HandleCANRPDO(void) {
	if (receiveIndex == numberRPDO) { //= bus available
		busState = 1; //bus free for sending
	} else if (receiveIndex < numberRPDO) { //receive smt--> interpret msg
		RxMessage = RetrieveFromQueue();
		busState = 2;
		if (RxMessage != NULL) {
			u32 temp;
			command = DetermineCommand(RxMessage);
			switch (command) {
			case BoxInfoRQ:
				SendBoxState();
				break;
			case WeightRQ:
				SendPepperState();
				break;
			case BatteryRQ:
				SendBatteryState();
				break;
			case Save2Flash:
				configFlashWrite();
				break;
			case setNOWEIGHTLV:
				temp = ConvertU8ToU32(RxMessage->Data, 2);
				configSetParamByID(setNOWEIGHTLV, (float) temp);
				break;
			case setEMPTYWEIGHTLV:
				temp = ConvertU8ToU32(RxMessage->Data, 2);
				configSetParamByID(setEMPTYWEIGHTLV, (float) temp);
				break;
			case setFULLWEIGHTLV:
				temp = ConvertU8ToU32(RxMessage->Data, 2);
				configSetParamByID(setFULLWEIGHTLV, (float) temp);
				break;
			case setWEIGHTTOLLERANCE:
				temp = ConvertU8ToU32(RxMessage->Data, 2);
				configSetParamByID(setWEIGHTTOLLERANCE, (float) temp);
				break;
			case setCONFIG_VERSION:
				temp = ConvertU8ToU32(RxMessage->Data, 2);
				configSetParamByID(setCONFIG_VERSION, (float) temp);
				break;
			case getNOWEIGHTLV:
				SendNOWEIGHTLV();
				break;
			case getEMPTYWEIGHTLV:
				SendEMPTYWEIGHTLV();
				break;
			case getFULLWEIGHTLV:
				SendFULLWEIGHTLV();
				break;
			case getWEIGHTTOLLERANCE:
				SendWEIGHTTOLLERANCE();
				break;
			case getCONFIG_VERSION:
				SendCONFIG_VERSION();
				break;
			}
			command = 0;
			RxMessage = NULL;
			receiveIndex++;
		}
	}
}

void HandleCANTPDO(void) {
}
