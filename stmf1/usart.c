#include "usart.h"

USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

serialPort_t serialPort3;

void SerialPortInit(serialPort_t ** mySerialPort, char portID) {
	*mySerialPort = (serialPort_t *) calloc(1, sizeof(serialPort_t));
	(*mySerialPort)->portID = 3;
	(*mySerialPort)->rxBufSize = (*mySerialPort)->txBufSize = SERIAL_BUFSIZE;
	(*mySerialPort)->txHead = (*mySerialPort)->txTail = 0;
	(*mySerialPort)->rxHead = (*mySerialPort)->rxTail = 0;
	(*mySerialPort)->txDmaRunning = 0;
}

void USARTConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	serialPort_t * s = &serialPort3;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE);

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USARTy_TxPin; // PA.09 USART1.TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USARTy_RxPin; // PA.10 USART1.RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

	DMA_DeInit(USARTy_Tx_DMA_Channel);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & USARTy->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) s->txBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = SERIAL_BUFSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(USARTy_Tx_DMA_Channel, &DMA_InitStructure);

	DMA_DeInit(USARTy_Rx_DMA_Channel);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & USARTy->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) s->rxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = SERIAL_BUFSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(USARTy_Rx_DMA_Channel, &DMA_InitStructure);

	DMA_ITConfig(USARTy_Tx_DMA_Channel, DMA_IT_TC | DMA_IT_TE, ENABLE);
	DMA_ITConfig(USARTy_Rx_DMA_Channel, DMA_IT_TC | DMA_IT_TE, ENABLE);
	DMA_Cmd(USARTy_Tx_DMA_Channel, ENABLE);
	DMA_Cmd(USARTy_Rx_DMA_Channel, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_TX_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_RX_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTy, &USART_InitStructure);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

unsigned char serialAvailable(serialPort_t *s) {
	return (USARTy_Rx_DMA_Channel->CNDTR != s->rxPos);
}

//the next 2 functions is suitable for streaming
void serialPrint(serialPort_t *s, const char *str) {
	while (*str)
		serialWrite(s, *(str++));
}
void serialWrite(serialPort_t *s, unsigned char ch) {
	s->txBuf[s->txHead] = ch;
	s->txHead = (s->txHead + 1) % s->txBufSize;
	serialStartTxDMA(s);
}
void serialStartTxDMA(serialPort_t *s) {
	if (!s->txDmaRunning) {
		USARTy_Tx_DMA_Channel->CMAR = (uint32_t) & s->txBuf[s->txTail]; //TODO:?????????
		if (s->txHead > s->txTail) {
			USARTy_Tx_DMA_Channel->CNDTR = s->txHead - s->txTail;
			s->txTail = s->txHead;
		} else {
			USARTy_Tx_DMA_Channel->CNDTR = s->txBufSize - s->txTail;
			s->txTail = 0;
		}
		s->txDmaRunning = 1;
		DMA_Cmd(USARTy_Tx_DMA_Channel, ENABLE);
	}
}

//the next 2 funcs re suitable for send 1 block of msg
void serialWrite1(unsigned char ch, size_t stringSize) {
	serialPort_t *s = &serialPort3;
	while (s->txPos > 0) {
	}
	DMA_Cmd(USARTy_Tx_DMA_Channel, DISABLE);
	memcpy(s->txBuf, &ch, 1);
	DMA_SetCurrDataCounter(USARTy_Tx_DMA_Channel, 1);
	DMA_Cmd(USARTy_Tx_DMA_Channel, ENABLE);
}
int serialPrint1(char* str, size_t stringSize) {
	serialPort_t *s = &serialPort3;
	size_t stringLength = stringSize;

	char* stringPointer = str;
	if (stringLength > SERIAL_BUFSIZE) {
		while (stringLength >= SERIAL_BUFSIZE) {
			while (s->txPos != 0 || s->txDmaRunning == 1) {
			}
			DMA_Cmd(USARTy_Tx_DMA_Channel, DISABLE);
			memcpy((s->txBuf + s->txPos), stringPointer, SERIAL_BUFSIZE);
			DMA_SetCurrDataCounter(USARTy_Tx_DMA_Channel, SERIAL_BUFSIZE);
			stringLength -= SERIAL_BUFSIZE;
			stringPointer += SERIAL_BUFSIZE;
			s->txDmaRunning = 1;
			DMA_Cmd(USARTy_Tx_DMA_Channel, ENABLE);
		}
	}

	while (s->txPos + stringLength >= SERIAL_BUFSIZE || s->txDmaRunning == 1) {
	}
	if (s->txPos + stringLength < SERIAL_BUFSIZE && s->txDmaRunning == 0) {
		DMA_Cmd(USARTy_Tx_DMA_Channel, DISABLE);
		s->txPos = DMA_GetCurrDataCounter(USARTy_Tx_DMA_Channel);
		if (s->txPos == 0) {
			memcpy((s->txBuf + s->txPos), stringPointer, stringLength);
		} else {
			memcpy((s->txBuf + s->txPos + 1), stringPointer, stringLength);
		}

		DMA_SetCurrDataCounter(USARTy_Tx_DMA_Channel, s->txPos + stringLength);
		DMA_Cmd(USARTy_Tx_DMA_Channel, ENABLE);

	}

	return 0;
}

void USARTx_DMA_TX_IRQHandler(void) {
	serialPort_t *s = &serialPort3;
	if (DMA_GetFlagStatus(DMA1_IT_TC4) == SET) { // ensure check interrupt flag for receive data
		s->txDmaRunning = 0;
		s->txPos = DMA_GetCurrDataCounter(DMA1_Channel4);
		DMA_ClearITPendingBit(DMA1_IT_TC4); /// Clear Interrupt pending bit
	}
}
unsigned int serialPrint2(char* str, size_t stringSize) {
	serialPort_t *s = &serialPort3;
	if (s->txPos == 0) {
		size_t stringLength = stringSize;
		char* stringPointer = str;

		DMA_Cmd(USARTy_Tx_DMA_Channel, DISABLE);
		memcpy((s->txBuf), stringPointer, stringLength);
		s->txDmaRunning = 1;
		s->txPos += stringSize;
		DMA_SetCurrDataCounter(USARTy_Tx_DMA_Channel, stringLength);
		DMA_Cmd(USARTy_Tx_DMA_Channel, ENABLE);
	}
	return s->txPos;
}
