#ifndef		__cUart_h
#define 	__cUart_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "cRingBuffer.h"
//#include "myLib/Assert.h"
#include <string.h>
#include <stdarg.h>

#define UART_BUFSIZE					256

#define 	BAUD 	115200
//#define 	BAUD 	57600
//#define 	BAUD 	38400
//#define 	BAUD 	19200
//#define 	BAUD 	9600s

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
#define USARTx_RX_DMA_STREAM            DMA1_Stream1
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

#define USARTx_DMA_TX_IRQn              DMA1_Stream3_IRQn
#define USARTx_DMA_RX_IRQn              DMA1_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler        DMA1_Stream3_IRQHandler
#define USARTx_DMA_RX_IRQHandler        DMA1_Stream1_IRQHandler

#endif

typedef struct {
	int portID;
	int baudrate;
	USART_TypeDef* thisUsart;
	DMA_Stream_TypeDef* txDMAStream, *rxDMAStream;
	int txBufSize, rxBufSize;
	char txBuf[UART_BUFSIZE];
	char rxBuf[UART_BUFSIZE];
	RingBuffer * rxRingBuff;
} UartPort;

void UartPortStructInit(UartPort** me);
int IsUartReady(UartPort* me);
int GetNumberByteLeftToSend(UartPort* me);
void UartPrint(char* data, ...);
void cUartConfig(int comX);
char ReadUart();
#endif
