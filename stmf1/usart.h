#ifndef USART_H
#define USART_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "STM32_Nucleo/stm32f1xx/stm32f1xx_nucleo.h"
#define 	BAUD 	115200
//#define 	BAUD 	57600
//#define 	BAUD 	38400
//#define 	BAUD 	19200
//#define 	BAUD 	9600s

/** Definition for USARTx resources ********************************************/
#define USART 	1

#if USART == 1
#define USARTy                   USART1
#define USARTy_GPIO              GPIOA
#define USARTy_CLK               RCC_APB2Periph_USART1
#define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USARTy_RxPin             GPIO_Pin_10
#define USARTy_TxPin             GPIO_Pin_9
#define USARTy_Tx_DMA_Channel    DMA1_Channel4
#define USARTy_Tx_DMA_FLAG       DMA1_FLAG_TC4
#define USARTy_Rx_DMA_Channel    DMA1_Channel5
#define USARTy_Rx_DMA_FLAG       DMA1_FLAG_TC5
#define USARTx_DMA_TX_IRQHandler DMA1_Channel4_IRQHandler
#define USARTx_DMA_TX_IRQ		 DMA1_Channel4_IRQn
#define USARTx_DMA_RX_IRQHandler DMA1_Channel5_IRQHandler
#define USARTx_DMA_RX_IRQ		 DMA1_Channel5_IRQn

#define USARTz                   USART2
#define USARTz_GPIO              GPIOD
#define USARTz_CLK               RCC_APB1Periph_USART2
#define USARTz_GPIO_CLK          RCC_APB2Periph_GPIOD
#define USARTz_RxPin             GPIO_Pin_6
#define USARTz_TxPin             GPIO_Pin_5
#define USARTz_Tx_DMA_Channel    DMA1_Channel7
#define USARTz_Tx_DMA_FLAG       DMA1_FLAG_TC7
#define USARTz_Rx_DMA_Channel    DMA1_Channel6
#define USARTz_Rx_DMA_FLAG       DMA1_FLAG_TC6
#define USARTz_DR_Base           0x40004404

#define USARTt                   USART3
#define USARTt_GPIO              GPIOC
#define USARTt_CLK               RCC_APB1Periph_USART3
#define USARTt_GPIO_CLK          RCC_APB2Periph_GPIOC
#define USARTt_RxPin             GPIO_Pin_11
#define USARTt_TxPin             GPIO_Pin_10
#define USARTt_Tx_DMA_Channel    DMA1_Channel2
#define USARTt_Tx_DMA_FLAG       DMA1_FLAG_TC2
#define USARTt_Rx_DMA_Channel    DMA1_Channel3
#define USARTt_Rx_DMA_FLAG       DMA1_FLAG_TC3
#define USARTt_DR_Base           0x40004804
#endif
#define SERIAL_BUFSIZE                       128

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

void USARTConfig(void);
void SerialPortInit(serialPort_t ** mySerialPort, char portID);

unsigned char serialAvailable(serialPort_t *s);
void serialPrint(serialPort_t *s, const char *str);
void serialWrite(serialPort_t *s, unsigned char ch);
void serialStartTxDMA(serialPort_t *s);
void serialWrite1(unsigned char ch, size_t stringSize);
int serialPrint1(char* str, size_t stringSize);
//void DMA1_Stream3_IRQHandler(void);
extern void USARTx_DMA_TX_IRQHandler(void) ;
unsigned int serialPrint2(char* str, size_t stringSize);
#endif
