#include "cUart.h"
#include "myLib/cRingBuffer.h"
#include "utility.h"

UartPort* uartPort1;
UartPort* uartPort2;
int receivedCounter = 0;
void UartPortStructInit(UartPort** me) {
	memset(*me, 0, sizeof(UartPort));
}
void UartPort3StructConfig(UartPort ** me) {
	RingBuffer * rxRingBuff = malloc(sizeof(RingBuffer));
	(*me)->rxRingBuff = rxRingBuff;
	RingBuffer *queuePtr = (*me)->rxRingBuff;

	(*me)->portID = 3;
	(*me)->baudrate = 115200;
	(*me)->thisUsart = USART3;
	(*me)->txDMAStream = DMA1_Stream3;
	(*me)->rxDMAStream = DMA1_Stream1;
	(*me)->txBufSize = UART_BUFSIZE;
	(*me)->rxBufSize = UART_BUFSIZE;

	RingBufferStructInit(queuePtr);
	RingBufferConfig(queuePtr, UART_BUFSIZE, sizeof(char));
}
void Uart3Config(UartPort* me) {
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //tx
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //rx
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = (unsigned int) me->baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//USARTx_TX_DMA_STREAM
	DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) me->txBuf;
	DMA_InitStructure.DMA_BufferSize = me->txBufSize;
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
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	DMA_SetCurrDataCounter(DMA1_Stream3, 0);
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC | DMA_IT_FE | DMA_IT_TE, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, DISABLE);
	DMA_Cmd(DMA1_Stream3, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USARTx, ENABLE);
}

void UartPort6StructConfig(UartPort ** me) {
	RingBuffer * rxRingBuff = malloc(sizeof(RingBuffer));
	(*me)->rxRingBuff = rxRingBuff;
	RingBuffer *queuePtr = (*me)->rxRingBuff;

	(*me)->portID = 6;
	(*me)->baudrate = 115200;
	(*me)->thisUsart = USART6;
	(*me)->txDMAStream = DMA2_Stream6;
	(*me)->rxDMAStream = DMA2_Stream1;
	(*me)->txBufSize = UART_BUFSIZE;
	(*me)->rxBufSize = UART_BUFSIZE;

	RingBufferStructInit(queuePtr);
	RingBufferConfig(queuePtr, UART_BUFSIZE, sizeof(char));
}
void Uart6Config(UartPort* me) {
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //tx
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //rx
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = (unsigned int) me->baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(me->thisUsart, &USART_InitStructure);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

	//USARTx_TX_DMA_STREAM
	DMA_DeInit(me->txDMAStream);
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(me->thisUsart->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) me->txBuf;
	DMA_InitStructure.DMA_BufferSize = me->txBufSize;
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
	DMA_Init(me->txDMAStream, &DMA_InitStructure);
	DMA_SetCurrDataCounter(me->txDMAStream, 0);
	DMA_ITConfig(me->txDMAStream, DMA_IT_TC | DMA_IT_FE | DMA_IT_TE, ENABLE);
	USART_DMACmd(me->thisUsart, USART_DMAReq_Tx, DISABLE);
	DMA_Cmd(me->txDMAStream, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(me->thisUsart, ENABLE);
}
int IsUartReady(UartPort* me) {
	return DMA_GetCurrDataCounter(me->txDMAStream) == 0;
}
int GetNumberByteLeftToSend(UartPort* me) {
	return DMA_GetCurrDataCounter(me->txDMAStream);
}
void UartPrint6(char* data, ...) {
	while (!IsUartReady(uartPort1)) {
	}
	va_list args;
	va_start(args, data);
	vsnprintf(uartPort1->txBuf, UART_BUFSIZE, data, args);
	va_end(args);
	uartPort1->txDMAStream->NDTR = (uint16_t) strlen(uartPort1->txBuf);
	uartPort1->txDMAStream->CR |= (uint) DMA_SxCR_EN;
	uartPort1->thisUsart->CR3 |= USART_DMAReq_Tx;
}
void UartPrint(char* data, ...) {
	while (!IsUartReady(uartPort2)) {
	}
	va_list args;
	va_start(args, data);
	vsnprintf(uartPort2->txBuf, UART_BUFSIZE, data, args);
	va_end(args);
	uartPort2->txDMAStream->NDTR = (uint16_t) strlen(uartPort2->txBuf);
	uartPort2->txDMAStream->CR |= (uint) DMA_SxCR_EN;
	uartPort2->thisUsart->CR3 |= USART_DMAReq_Tx;
}
void UartPrintXX(char* data, ...) {
	while (!IsUartReady(uartPort1)) {
	}
	va_list args;
	va_start(args, data);
	vsnprintf(uartPort1->txBuf, UART_BUFSIZE, data, args);
	va_end(args);
	uartPort1->txDMAStream->NDTR = (uint16_t) strlen(uartPort1->txBuf);
	uartPort1->txDMAStream->CR |= (uint) DMA_SxCR_EN;
	uartPort1->thisUsart->CR3 |= USART_DMAReq_Tx;
}
char Uart6Read(UartPort *me) {
//	UartPrint("count:%d\r\n",uartPort1->rxRingBuff->active);
//	simpleDelay(100000000);

	char ret = '\0';
	int counts = GetNumberByteUsedOfRBuffer(uartPort1->rxRingBuff);
	if (counts != 0) {
		PopRingBuffer(uartPort1->rxRingBuff, &ret);
	}

	return ret;
}
char Uart3Read(UartPort *me) {
//	UartPrint("count:%d\r\n",uartPort1->rxRingBuff->active);
//	simpleDelay(100000000);

	char ret = '\0';
	int counts = GetNumberByteUsedOfRBuffer(uartPort2->rxRingBuff);
	if (counts != 0) {
		PopRingBuffer(uartPort2->rxRingBuff, &ret);
	}

	return ret;
}

void DMA1_Stream3_IRQHandler(void) { //tx
	UartPort* prt = uartPort2;
	if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET) {
		//disable DmaCmd, usartDMACMD
		prt->txDMAStream->CR &= ~(uint32_t) DMA_SxCR_EN;
		prt->thisUsart->CR3 &= (uint16_t) ~USART_DMAReq_Tx;
		DMA_ClearITPendingBit(prt->txDMAStream, DMA_IT_TCIF3); /// Clear Interrupt pending bit
	} else if (DMA_GetITStatus(prt->txDMAStream, DMA_IT_TEIF3) != RESET) {
		DMA_ClearITPendingBit(prt->txDMAStream, DMA_IT_TEIF3);
	} else if (DMA_GetITStatus(prt->txDMAStream, DMA_IT_FEIF3) != RESET) {
		DMA_ClearITPendingBit(prt->txDMAStream, DMA_IT_FEIF3);
	}
}
void USART3_IRQHandler(void) {
	char receivedBuff = '\0';
	UartPort* prt = uartPort2;
	if (USART_GetITStatus(prt->thisUsart, USART_IT_RXNE) != RESET) {
		receivedBuff = USART_ReceiveData(prt->thisUsart);
		PushRingBuffer(prt->rxRingBuff, (void*) &receivedBuff);
	} else if (USART_GetITStatus(prt->thisUsart, USART_IT_ORE_RX) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_ORE_ER) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_NE) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_FE) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_PE) != RESET) {
		UartPrint("Uart got error on receive\r\n");

		USART_ClearITPendingBit(prt->thisUsart, USART_IT_ORE_RX);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_ORE_ER);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_NE);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_FE);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_PE);
	}
}

void DMA2_Stream6_IRQHandler(void) { //tx
	UartPort* prt = uartPort1;
	if (DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6) != RESET) {
		//disable DmaCmd, usartDMACMD
		prt->txDMAStream->CR &= ~(uint32_t) DMA_SxCR_EN;
		prt->thisUsart->CR3 &= (uint16_t) ~USART_DMAReq_Tx;
		DMA_ClearITPendingBit(prt->txDMAStream, DMA_IT_TCIF6); /// Clear Interrupt pending bit
	} else if (DMA_GetITStatus(prt->txDMAStream, DMA_IT_TEIF6) != RESET) {
		DMA_ClearITPendingBit(prt->txDMAStream, DMA_IT_TEIF6);
	} else if (DMA_GetITStatus(prt->txDMAStream, DMA_IT_FEIF6) != RESET) {
		DMA_ClearITPendingBit(prt->txDMAStream, DMA_IT_FEIF6);
	}
}
void USART6_IRQHandler(void) {
	char receivedBuff = '\0';
	UartPort* prt = uartPort1;
	if (USART_GetITStatus(prt->thisUsart, USART_IT_RXNE) != RESET) {
		receivedBuff = USART_ReceiveData(prt->thisUsart);
		PushRingBuffer(prt->rxRingBuff, (void*) &receivedBuff);
	} else if (USART_GetITStatus(prt->thisUsart, USART_IT_ORE_RX) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_ORE_ER) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_NE) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_FE) != RESET
			|| USART_GetITStatus(prt->thisUsart, USART_IT_PE) != RESET) {
		UartPrint("Uart got error on receive\r\n");

		USART_ClearITPendingBit(prt->thisUsart, USART_IT_ORE_RX);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_ORE_ER);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_NE);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_FE);
		USART_ClearITPendingBit(prt->thisUsart, USART_IT_PE);
	}
}

void cUartConfig(int comX) {
	if (comX == 3) {
		uartPort2 = malloc(sizeof(UartPort));
		UartPortStructInit(&uartPort2);
		UartPort3StructConfig(&uartPort2);
		Uart3Config(uartPort2);
	} else if (comX == 6) {
		uartPort1 = malloc(sizeof(UartPort));
		UartPortStructInit(&uartPort1);
		UartPort6StructConfig(&uartPort1);
		Uart6Config(uartPort1);
	}
}
char ReadUart() {
	return Uart3Read(uartPort2);
}
