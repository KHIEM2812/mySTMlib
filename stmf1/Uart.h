#ifndef _usart_h
#define _usart_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "STM32F4-Discovery/stm32f4_discovery.h"

#define 	BAUD 	115200
//#define 	BAUD 	57600
//#define 	BAUD 	38400
//#define 	BAUD 	19200
//#define 	BAUD 	9600s

#define SERIAL_BUFSIZE					256

#define SERIALCOMx							3

#if SERIALCOMx == 6
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

#define USARTx_TX_DMA_IT_FEIF         	DMA_IT_FEIF6
#define USARTx_TX_DMA_IT_DMEIF        	DMA_IT_DMEIF6
#define USARTx_TX_DMA_IT_TEIF         	DMA_IT_TEIF6
#define USARTx_TX_DMA_IT_HTIF         	DMA_IT_HTIF6
#define USARTx_TX_DMA_IT_TCIF         	DMA_IT_TCIF6

#define USARTx_RX_DMA_CHANNEL           DMA_Channel_5
#define USARTx_RX_DMA_STREAM            DMA2_Stream1
#define USARTx_RX_DMA_FLAG_FEIF         DMA_FLAG_FEIF1
#define USARTx_RX_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF1
#define USARTx_RX_DMA_FLAG_TEIF         DMA_FLAG_TEIF1
#define USARTx_RX_DMA_FLAG_HTIF         DMA_FLAG_HTIF1
#define USARTx_RX_DMA_FLAG_TCIF         DMA_FLAG_TCIF1

#define USARTx_RX_DMA_IT_FEIF         	DMA_IT_FEIF1
#define USARTx_RX_DMA_IT_DMEIF        	DMA_IT_DMEIF1
#define USARTx_RX_DMA_IT_TEIF         	DMA_IT_TEIF1
#define USARTx_RX_DMA_IT_HTIF         	DMA_IT_HTIF1
#define USARTx_RX_DMA_IT_TCIF         	DMA_IT_TCIF1

#define USARTx_DMA_TX_IRQn              DMA2_Stream6_IRQn
#define USARTx_DMA_RX_IRQn              DMA2_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler        DMA2_Stream6_IRQHandler
#define USARTx_DMA_RX_IRQHandler        DMA2_Stream1_IRQHandler

#elif	SERIALCOMx == 3
#define USARTx							USART3
#define USARTx_CLK_EN                   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE)
#define USARTx_IRQn                     USART3_IRQn
#define USARTx_IRQHandler               USART3_IRQHandler

#define USARTx_TX_PIN                   GPIO_Pin_10
#define USARTx_TX_GPIO_PORT             GPIOB
#define USARTx_TX_GPIO_CLK_EN           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define USARTx_TX_AFConfig 				GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3)

#define USARTx_RX_PIN                   GPIO_Pin_11
#define USARTx_RX_GPIO_PORT             GPIOB
#define USARTx_RX_GPIO_CLK_EN           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define USARTx_RX_AFConfig              GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3)

#define USARTx_DR_ADDRESS               ((uint32_t)&UART3->DR)
#define USARTx_DMA                      DMA1
#define USARTx_DMAx_CLK_EN              RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

#define USARTx_TX_DMA_CHANNEL           DMA_Channel_4
#define USARTx_TX_DMA_STREAM            DMA1_Stream3
#define USARTx_TX_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
#define USARTx_TX_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
#define USARTx_TX_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
#define USARTx_TX_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
#define USARTx_TX_DMA_FLAG_TCIF         DMA_FLAG_TCIF3

#define USARTx_TX_DMA_IT_FEIF         	DMA_IT_FEIF3
#define USARTx_TX_DMA_IT_DMEIF        	DMA_IT_DMEIF3
#define USARTx_TX_DMA_IT_TEIF         	DMA_IT_TEIF3
#define USARTx_TX_DMA_IT_HTIF         	DMA_IT_HTIF3
#define USARTx_TX_DMA_IT_TCIF         	DMA_IT_TCIF3

#define USARTx_RX_DMA_CHANNEL           DMA_Channel_4
#define USARTx_RX_DMA_STREAM            DMA1_Stream2
#define USARTx_RX_DMA_FLAG_FEIF         DMA_FLAG_FEIF2
#define USARTx_RX_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF2
#define USARTx_RX_DMA_FLAG_TEIF         DMA_FLAG_TEIF2
#define USARTx_RX_DMA_FLAG_HTIF         DMA_FLAG_HTIF2
#define USARTx_RX_DMA_FLAG_TCIF         DMA_FLAG_TCIF2

#define USARTx_RX_DMA_IT_FEIF         	DMA_IT_FEIF2
#define USARTx_RX_DMA_IT_DMEIF        	DMA_IT_DMEIF2
#define USARTx_RX_DMA_IT_TEIF         	DMA_IT_TEIF2
#define USARTx_RX_DMA_IT_HTIF         	DMA_IT_HTIF2
#define USARTx_RX_DMA_IT_TCIF         	DMA_IT_TCIF2


#define USARTx_DMA_TX_IRQn              DMA1_Stream3_IRQn
#define USARTx_DMA_RX_IRQn              DMA1_Stream2_IRQn
#define USARTx_DMA_TX_IRQHandler        DMA1_Stream3_IRQHandler
#define USARTx_DMA_RX_IRQHandler        DMA1_Stream2_IRQHandler

#endif

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
void myPrintf2(char* data, ...);
void myPrintf3(char* data, ...);
void myPrintf(char* data, int len);
void myPutChar(char data);
void UART_PutChar(char data);
void serialPrintf1(void);
#endif
