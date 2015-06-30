/**\class usart6
 *
 * \ingroup KineticFarcade
 *
 * \brief Configure the hardware of usart6 + simple putchar, sendchar + ringBuffer
 *
 * \note Require mapPrinf class to fully implement printf
 *
 * \author DO HOANG DUY KHIEM
 *
 * \version 2.0
 *
 * \date 6/3/2014
 *
 * Contact: khiemdo@hoptechnik.com
 */


#ifndef USART_H_
#define USART_H_

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "usart6.h"
#include "misc.h"

// CONST
#define 	COMBUF_SIZE_TX		256
#define 	COMBUF_SIZE_RX		256

#define 	BAUD 	115200
//#define 	BAUD 	57600
//#define 	BAUD 	38400
//#define 	BAUD 	19200
//#define 	BAUD 	9600


#define RingbBytesInBuf(rb)		((rb)->cnt)
#define RingbBytesFree(rb)		((rb)->size - (rb)->cnt)


#define	_kbhit()		uart_rx()
#define	_getch()		uart_getch()
#define	gets(b)			uart_gets(b,sizeof(b))

#ifdef putchar
#undef putchar
#define putchar(ch)		uart_putchar(ch)
#endif


/** USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted.
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, frequence
   CPU...). */
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/** USART Mode Selected */
#define USART_MODE_TRANSMITTER           0x00
#define USART_MODE_RECEIVER              0x01

/** USART Transaction Type */
#define USART_TRANSACTIONTYPE_CMD        0x00
#define USART_TRANSACTIONTYPE_DATA       0x01


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
#endif


typedef struct
{
	volatile uint16_t	wp;				// write idx
	volatile uint16_t	rp;				// read idx
	volatile uint16_t	cnt;			// bytes in buffer
	volatile uint16_t	size;			// buffer size
	uint8_t	buf[0];
} st_ringbuf;


void UartInit(void);
void USARTx_IRQHandler(void);

void RingbInit(st_ringbuf *rb, uint16_t);
int8_t RingbWrite(st_ringbuf *rb, char d);
int8_t RingbWriteBuf(st_ringbuf *rb, uint8_t *buf, uint16_t anz);
int16_t RingbRead(st_ringbuf *rb);
int8_t RingbReadBuf(st_ringbuf *rb, uint8_t *buf, uint16_t anz);


void uart_send(const char *text, int len);
void uart_putc(char ch);
void uart_putchar(char ch);
void uart_puts(const char *text);
void uart_putcrlf(void);

void uart_receive(char *buf, int cnt);
char *uart_gets(char *buf, int size);
int uart_getch(void);
int uart_rx(void);

#endif


