#include "usart.h"

USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

serialPort_t *serialPort3;
//command myCommand;

extern char myString[200];
void SerialPortInit(serialPort_t ** mySerialPort, char portID) {
	*mySerialPort = (serialPort_t *) calloc(1, sizeof(serialPort_t));
	(*mySerialPort)->portID = 3;
	(*mySerialPort)->rxBufSize = (*mySerialPort)->txBufSize = SERIAL_BUFSIZE;
	(*mySerialPort)->txHead = (*mySerialPort)->txTail = 0;
	(*mySerialPort)->rxHead = (*mySerialPort)->rxTail = 0;
	(*mySerialPort)->txDmaRunning = 0;
}

void USART_Config(void) {
//	myCommand.charIn = 0;
//	//myCommand.cmdBody = NULL;
//	myCommand.msgid = 0;
//	myCommand.payloadCount = 0;
//	myCommand.state = 0;

	SerialPortInit(&serialPort3, 3);
	serialPort_t *s;
	s = serialPort3;

	RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
	USARTx_CLK_INIT(USARTx_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(USARTx_DMAx_CLK, ENABLE);

	/* Connect USART pins to AF7 */
	GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
	GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
	GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
	GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USARTx configuration ----------------------------------------------------*/
	USART_InitStructure.USART_BaudRate = BAUD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);

	DMA_DeInit(USARTx_TX_DMA_STREAM);
	DMA_InitStructure.DMA_Channel = USARTx_TX_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USARTx->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) serialPort3->txBuf;
	DMA_InitStructure.DMA_BufferSize = SERIAL_BUFSIZE;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USARTx_TX_DMA_STREAM, &DMA_InitStructure);
	DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, 0);
	DMA_ITConfig(USARTx_TX_DMA_STREAM, DMA_IT_TC, ENABLE);
	USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

	/* Configure DMA controller to manage USART RX request ----------*/
	DMA_DeInit(USARTx_RX_DMA_STREAM);
	DMA_InitStructure.DMA_Channel = USARTx_RX_DMA_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & USARTx->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) s->rxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = SERIAL_BUFSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USARTx_RX_DMA_STREAM, &DMA_InitStructure);
	DMA_ClearFlag(USARTx_RX_DMA_STREAM,
			USARTx_RX_DMA_FLAG_TCIF | USARTx_RX_DMA_FLAG_HTIF
					| USARTx_RX_DMA_FLAG_TEIF);

	USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
	s->rxPos = DMA_GetCurrDataCounter(USARTx_RX_DMA_STREAM);

	//DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
	DMA_Cmd(USARTx_RX_DMA_STREAM, ENABLE);

	/* NVIC configuration ----------------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USARTx, ENABLE);

}

unsigned char serialAvailable(serialPort_t *s) {
	return (USARTx_RX_DMA_STREAM->NDTR != s->rxPos);
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
		USARTx_TX_DMA_STREAM->M0AR = (uint32_t) & s->txBuf[s->txTail];
		if (s->txHead > s->txTail) {
			USARTx_TX_DMA_STREAM->NDTR = s->txHead - s->txTail;
			s->txTail = s->txHead;
		} else {
			USARTx_TX_DMA_STREAM->NDTR = s->txBufSize - s->txTail;
			s->txTail = 0;
		}
		s->txDmaRunning = 1;
		DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
	}
}

//the next 2 funcs re suitable for send 1 block of msg
void serialWrite1(unsigned char ch, size_t stringSize) {
	serialPort_t *s = serialPort3;
	while (s->txPos > 0) {
	}
	DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);
	memcpy(s->txBuf, &ch, 1);
	DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, 1);
	DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
}
int serialPrint1(char* str, size_t stringSize) {
	serialPort_t *s = serialPort3;
	size_t stringLength = stringSize;
	char* stringPointer = str;
	if (stringLength > SERIAL_BUFSIZE) {
		while (stringLength >= SERIAL_BUFSIZE) {
			while (s->txPos != 0 || serialPort3->txDmaRunning == 1) {
			}
			DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);
			memcpy((s->txBuf + s->txPos), stringPointer, SERIAL_BUFSIZE);
			DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, SERIAL_BUFSIZE);
			stringLength -= SERIAL_BUFSIZE;
			stringPointer += SERIAL_BUFSIZE;
			serialPort3->txDmaRunning = 1;
			DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
		}
	}

	while (s->txPos + stringLength >= SERIAL_BUFSIZE
			|| serialPort3->txDmaRunning == 1) {
	}
	if (s->txPos + stringLength < SERIAL_BUFSIZE
			&& serialPort3->txDmaRunning == 0) {
		DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);
		serialPort3->txPos = DMA_GetCurrDataCounter(USARTx_TX_DMA_STREAM);
		if (s->txPos == 0) {
			memcpy((s->txBuf + s->txPos), stringPointer, stringLength);
		} else {
			memcpy((s->txBuf + s->txPos + 1), stringPointer, stringLength);
		}

		DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, s->txPos + stringLength);
		DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);

	}

	return 0;
}

void USARTx_DMA_TX_IRQHandler(void) {
	if (DMA_GetFlagStatus(USARTx_TX_DMA_STREAM, USARTx_TX_DMA_FLAG_TCIF)
			== SET) { // ensure check interrupt flag for receive data
		serialPort3->txDmaRunning = 0;
		serialPort3->txPos = DMA_GetCurrDataCounter(USARTx_TX_DMA_STREAM);
		DMA_ClearITPendingBit(USARTx_TX_DMA_STREAM, USARTx_TX_DMA_FLAG_TCIF); /// Clear Interrupt pending bit
	}
}
unsigned int serialPrint2(char* str, size_t stringSize) {
	serialPort_t *s = serialPort3;
	if (s->txPos == 0) {
		size_t stringLength = stringSize;
		char* stringPointer = str;

//		while (s->txPos != 0 || serialPort3->txDmaRunning == 1) {
//		}
		DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);
		memcpy((s->txBuf), stringPointer, stringLength);
		serialPort3->txDmaRunning = 1;
		serialPort3->txPos += stringSize;
		DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, stringLength);
		DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
	}
	return serialPort3->txPos;
}

int serialRead(serialPort_t *s) {
	int ch;

	ch = s->rxBuf[s->rxBufSize - s->rxPos];
	if (--s->rxPos == 0)
		s->rxPos = s->rxBufSize;

	return ch;
}

