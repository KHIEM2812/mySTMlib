#ifndef _usart_h
#define _usart_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "STM32F4-Discovery/stm32f4_discovery.h"

#define 	BAUD 	115200
//#define 	BAUD 	57600
//#define 	BAUD 	38400
//#define 	BAUD 	19200
//#define 	BAUD 	9600s

#define SERIAL_BUFSIZE					256

#define USARTx							USART6
#define USARTx_CLK_EN                   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE)
#define USARTx_IRQn                     USART6_IRQn
#define USARTx_IRQHandler               USART6_IRQHandler

#define USARTx_TX_PIN                   GPIO_Pin_6
#define USARTx_TX_GPIO_PORT             GPIOC
#define USARTx_TX_GPIO_CLK_EN           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define USARTx_TX_AFConfig 				GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6)

#define USARTx_RX_PIN                   GPIO_Pin_7
#define USARTx_RX_GPIO_PORT             GPIOC
#define USARTx_RX_GPIO_CLK_EN           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define USARTx_RX_AFConfig              GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6)

#define USARTx_DR_ADDRESS               ((uint32_t)&UART6->DR)
#define USARTx_DMA                      DMA2
#define USARTx_DMAx_CLK_EN              RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

#define USARTx_TX_DMA_CHANNEL           DMA_Channel_5
#define USARTx_TX_DMA_STREAM            DMA2_Stream6
#define USARTx_TX_DMA_FLAG_FEIF         DMA_FLAG_FEIF6
#define USARTx_TX_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF6
#define USARTx_TX_DMA_FLAG_TEIF         DMA_FLAG_TEIF6
#define USARTx_TX_DMA_FLAG_HTIF         DMA_FLAG_HTIF6
#define USARTx_TX_DMA_FLAG_TCIF         DMA_FLAG_TCIF6

#define USARTx_RX_DMA_CHANNEL           DMA_Channel_5
#define USARTx_RX_DMA_STREAM            DMA2_Stream1
#define USARTx_RX_DMA_FLAG_FEIF         DMA_FLAG_FEIF1
#define USARTx_RX_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF1
#define USARTx_RX_DMA_FLAG_TEIF         DMA_FLAG_TEIF1
#define USARTx_RX_DMA_FLAG_HTIF         DMA_FLAG_HTIF1
#define USARTx_RX_DMA_FLAG_TCIF         DMA_FLAG_TCIF1

#define USARTx_DMA_TX_IRQn              DMA2_Stream6_IRQn
#define USARTx_DMA_RX_IRQn              DMA2_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler        DMA2_Stream6_IRQHandler
#define USARTx_DMA_RX_IRQHandler        DMA2_Stream1_IRQHandler

typedef struct {
	char portID;
	uint rxSem;
	unsigned int rxBufSize;
	volatile unsigned int rxHead, rxTail;
	volatile unsigned int rxPos;
	char txBuf[SERIAL_BUFSIZE];
	char rxBuf[SERIAL_BUFSIZE];
	USART_TypeDef* thisUsart;
} serialPort_t;

void UartConfig();
inline void SerialPortInit(serialPort_t ** mySerialPort, char portID);
void SerialPortStructInit(serialPort_t ** mySerialPort);

char IsUsartTransferComplete(serialPort_t *s);

void myPrintf1(char* data);
void myPrintf(char* data, int len);
void UART_PutChar(char data);
void serialPrintf1(void);
#endif
