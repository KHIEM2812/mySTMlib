#include "TUart.h"
extern serialPort_t *serialPort3;

extern serialPort_t* serialPort3;
void TUartDMA() {
	sprintf(serialPort3->txBuf, "hello\r\n");
	int length = strlen(serialPort3->txBuf);
	DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, length);
	DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
	USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
}

void TUartDMAWithStaticString() {
	myPrintf1("hello1 hello2 hello3 hello4 hello5 hello6 hello7\r\n  ");
}

void TPrinf() {
	static int i = 0;
	i = i + 1;
	printf("%d \r\n", i);
}

void TPrinfFloat() {
	static float i = 0.0;
	i = i + 1;
	printf("\r\n%.1f", i);
//	printf("\r\n%f",i);//not work fffffff
	simpleDelay(20000000);
}

void TMyPrintf() {
	char myString[255];
	sprintf(myString, "hello1 hello2 hello3 hello4 hello5 hello6 hello7\r\n  ");
	uint len = strlen(myString);
	myPrintf(myString, len);
}

void TMyPrintfWithFloat() {
	static float i = 0.0;
	i = i + 1;
	char myString[255];
	sprintf(myString, "%f\r\n", i);
	myPrintf1(myString);
}

void TIsUsartTransferCompletePrint(char* data, int len) {
	while (!IsUsartTransferComplete(serialPort3)) {
	}
	sprintf(serialPort3->txBuf, data);
	USARTx_TX_DMA_STREAM->NDTR = (uint16_t) strlen(serialPort3->txBuf);
	USARTx_TX_DMA_STREAM->CR |= (uint) DMA_SxCR_EN;
	USARTx->CR3 |= USART_DMAReq_Tx;
}

void TIsUsartTransferComplete() {
	static float i = 0.0;
	i = i + 1;
	char myString[255];
	sprintf(myString, "%f\r\n", i);
	TIsUsartTransferCompletePrint(myString, strlen(myString));
}

void TUART_PutChar() {
	UART_PutChar('h');
	UART_PutChar('a');
	UART_PutChar('h');
	UART_PutChar('0');
	simpleDelay(2000000);
}
