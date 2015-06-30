#ifndef USART_H
#define USART_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "STM32F4-Discovery/stm32f4_discovery.h"

#define 	BAUD 	115200
//#define 	BAUD 	57600
//#define 	BAUD 	38400
//#define 	BAUD 	19200
//#define 	BAUD 	9600s

/** Definition for USARTx resources ********************************************/
#define USART 	6

#if USART == 6
#define USARTx                           USART6
#define USARTx_CLK                       RCC_APB2Periph_USART6
#define USARTx_CLK_INIT                  RCC_APB2PeriphClockCmd
#define USARTx_IRQn                      USART6_IRQn
#define USARTx_IRQHandler                USART6_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_6
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USARTx_TX_SOURCE                 GPIO_PinSource6
#define USARTx_TX_AF                     GPIO_AF_USART6

#define USARTx_RX_PIN                    GPIO_Pin_7
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USARTx_RX_SOURCE                 GPIO_PinSource7
#define USARTx_RX_AF                     GPIO_AF_USART6

#define USARTx_DR_ADDRESS                ((uint32_t)&UART6->DR) //??????
#define USARTx_DMA                       DMA2
#define USARTx_DMAx_CLK                  RCC_AHB1Periph_DMA2

#define USARTx_TX_DMA_CHANNEL            DMA_Channel_5
#define USARTx_TX_DMA_STREAM             DMA2_Stream6
#define USARTx_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF6
#define USARTx_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF6
#define USARTx_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF6
#define USARTx_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF6
#define USARTx_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF6

#define USARTx_RX_DMA_CHANNEL            DMA_Channel_5
#define USARTx_RX_DMA_STREAM             DMA2_Stream1
#define USARTx_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF1
#define USARTx_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF1
#define USARTx_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF1
#define USARTx_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF1
#define USARTx_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF1

#define USARTx_DMA_TX_IRQn               DMA2_Stream6_IRQn
#define USARTx_DMA_RX_IRQn               DMA2_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler         DMA2_Stream6_IRQHandler
#define USARTx_DMA_RX_IRQHandler         DMA2_Stream1_IRQHandler

#elif USART == 3
#define USARTx                           USART3
#define USARTx_CLK                       RCC_APB1Periph_USART3
#define USARTx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define USARTx_IRQn                      USART3_IRQn
#define USARTx_IRQHandler                USART3_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_10
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define USARTx_TX_SOURCE                 GPIO_PinSource10
#define USARTx_TX_AF                     GPIO_AF_USART3

#define USARTx_RX_PIN                    GPIO_Pin_11
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define USARTx_RX_SOURCE                 GPIO_PinSource11
#define USARTx_RX_AF                     GPIO_AF_USART3

#define USARTx_DR_ADDRESS                ((uint32_t)&UART3->DR)								//((uint32_t)UART3 + 0x004) //??????
#define USARTx_DMA                       DMA1
#define USARTx_DMAx_CLK                  RCC_AHB1Periph_DMA1

#define USARTx_TX_DMA_CHANNEL            DMA_Channel_4
#define USARTx_TX_DMA_STREAM             DMA1_Stream3
#define USARTx_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF3
#define USARTx_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF3
#define USARTx_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF3
#define USARTx_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF3
#define USARTx_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF3

#define USARTx_RX_DMA_CHANNEL            DMA_Channel_4
#define USARTx_RX_DMA_STREAM             DMA1_Stream1
#define USARTx_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF1
#define USARTx_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF1
#define USARTx_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF1
#define USARTx_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF1
#define USARTx_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF1

#define USARTx_DMA_TX_IRQn               DMA1_Stream3_IRQn
#define USARTx_DMA_RX_IRQn               DMA1_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler         DMA1_Stream3_IRQHandler
#define USARTx_DMA_RX_IRQHandler         DMA1_Stream1_IRQHandler
#endif

#define SERIAL_BUFSIZE                       512
#define STATE_WAIT_SYNC1	0
#define STATE_WAIT_SYNC2	1
#define STATE_WAIT_LENGTH	2
#define STATE_WAIT_MSGID	3
#define STATE_CHECKSUM		4
#define STATE_READ_PAYLOAD	5

#define SYNC1 		0x7f
#define SYNC2		0xb2

typedef struct {
	char portID;
	char txBuf[SERIAL_BUFSIZE];
	unsigned int txBufSize;
	volatile unsigned int txHead, txTail;
	char rxBuf[SERIAL_BUFSIZE];
	unsigned int rxBufSize;
	volatile unsigned int rxHead, rxTail;
	volatile unsigned int rxPos, txPos;
	unsigned int txDmaRunning;
} serialPort_t;
//
//typedef struct{
//	char charIn;
//	char state;
//	char msgid;
//	char length;
//	char cmdBody[50];
//	char payloadCount;
//
//} command;



void USART_Config(void);
void SerialPortInit(serialPort_t ** mySerialPort, char portID);

unsigned char serialAvailable(serialPort_t *s);
void serialPrint(serialPort_t *s, const char *str);
void serialWrite(serialPort_t *s, unsigned char ch);
void serialStartTxDMA(serialPort_t *s);
void serialWrite1(unsigned char ch, size_t stringSize);
int serialPrint1(char* str, size_t stringSize);
void DMA1_Stream3_IRQHandler(void);
unsigned int serialPrint2(char* str, size_t stringSize);

int serialRead(serialPort_t *s);
void HandleRPDO(void);



#endif
