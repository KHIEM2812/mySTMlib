#include "Uart.h"

USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

serialPort_t *serialPort3;

inline void SerialPortInit(serialPort_t ** mySerialPort, char portID) {
	*mySerialPort = (serialPort_t *) calloc(1, sizeof(serialPort_t));
	(*mySerialPort)->portID = 3;
	(*mySerialPort)->rxBufSize = SERIAL_BUFSIZE;
	(*mySerialPort)->rxHead = (*mySerialPort)->rxTail = 0;
}

void UartConfig() {
	SerialPortInit(&serialPort3, 3);
	serialPort_t *s;
	s = serialPort3;
	serialPort3->thisUsart = USARTx;

	USARTx_TX_GPIO_CLK_EN;
	USARTx_RX_GPIO_CLK_EN;
	USARTx_DMAx_CLK_EN;
	USARTx_CLK_EN;
	USARTx_DMAx_CLK_EN;

	USARTx_TX_AFConfig;
	USARTx_RX_AFConfig;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
	GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
	GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

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
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USARTx_TX_DMA_STREAM, &DMA_InitStructure);
	DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, 0);
	DMA_ITConfig(USARTx_TX_DMA_STREAM, DMA_IT_TC | DMA_IT_FE | DMA_IT_TE,
			ENABLE);
	USART_DMACmd(USARTx, USART_DMAReq_Tx, DISABLE);
	DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);

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
	DMA_Cmd(USARTx_RX_DMA_STREAM, ENABLE);
	USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USARTx, ENABLE);

}

char IsUsartTransferComplete(serialPort_t *s) {
	return USART_GetFlagStatus(s->thisUsart, USART_FLAG_TC) == SET;
}

unsigned char serialAvailable(serialPort_t *s) {
	return (DMA_GetCurrDataCounter(USARTx_TX_DMA_STREAM) == 0);
}

int serialRead(serialPort_t *s) {
	int ch;

	ch = s->rxBuf[s->rxBufSize - s->rxPos];
	if (--s->rxPos == 0)
		s->rxPos = s->rxBufSize;

	return ch;
}

void myPrintf1(char* data) {
	while (!IsUsartTransferComplete(serialPort3)) {
	}
	sprintf(serialPort3->txBuf, data);
	USARTx_TX_DMA_STREAM->NDTR = (uint16_t) strlen(serialPort3->txBuf);
	USARTx_TX_DMA_STREAM->CR |= (uint) DMA_SxCR_EN;
	USARTx->CR3 |= USART_DMAReq_Tx;
}

void myPrintf(char* data, int len) {
	while (!IsUsartTransferComplete(serialPort3)) {
	}
	sprintf(serialPort3->txBuf, data);
	USARTx_TX_DMA_STREAM->NDTR = (uint16_t) strlen(serialPort3->txBuf);
	USARTx_TX_DMA_STREAM->CR |= (uint) DMA_SxCR_EN;
	USARTx->CR3 |= USART_DMAReq_Tx;

}

//not working!!!!!!!!!!!
void UART_PutChar(char data) {
	while (!IsUsartTransferComplete(serialPort3)) {
	}
	sprintf(serialPort3->txBuf, &data);
	u16 retCounter = DMA_GetCurrDataCounter(USARTx_TX_DMA_STREAM);
	DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);
	DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM,
			retCounter + 1);
	DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
}

void DMA2_Stream6_IRQHandler(void) {
	if (DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6) != RESET) { // ensure check interrupt flag for receive data
		USARTx_TX_DMA_STREAM->CR &= ~(uint32_t) DMA_SxCR_EN;
		USARTx->CR3 &= (uint16_t) ~USART_DMAReq_Tx;
		DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6); /// Clear Interrupt pending bit
	} else if (DMA_GetITStatus(DMA2_Stream6, DMA_IT_TEIF6) != RESET) {
		DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TEIF6);
	} else if (DMA_GetITStatus(DMA2_Stream6, DMA_IT_FEIF6) != RESET) {
		DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_FEIF6);
	}
}

