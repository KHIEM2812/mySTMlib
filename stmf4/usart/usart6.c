#include "usart6.h"

//#define	rxRingb		((st_ringbuf *)(&rxBuf[0]))
//#define	txRingb		((st_ringbuf *)(&txBuf[0]))

static uint8_t	txBuf[COMBUF_SIZE_TX + sizeof(st_ringbuf)];			// transmit buffer
static uint8_t	rxBuf[COMBUF_SIZE_RX + sizeof(st_ringbuf)];			// receive buffer

static st_ringbuf *rxRingb = (st_ringbuf *)rxBuf;
static st_ringbuf *txRingb = (st_ringbuf *)txBuf;

static volatile uint8_t fTxDisabled = 1;

#ifdef __GNUC__
  /** With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**--------------------------------------------------------------------
// RINGBUFFER Init (Constructor)
//--------------------------------------------------------------------
 */
void RingbInit(st_ringbuf *rb, uint16_t len)
{
	rb->rp	= 0;
	rb->wp	= 0;
	rb->cnt	= 0;
	rb->size= len - sizeof(st_ringbuf);
}

/**
// WRITE TO RINGBUFFER
 *
 */
static int8_t inline _RingbWrite(st_ringbuf *rb, char d)
{
	rb->buf[(rb->wp)++] = (uint8_t)d;
	if(rb->wp >= rb->size)
		rb->wp = 0;
	(rb->cnt)++;
	return 0;
}
int8_t RingbWrite(st_ringbuf *rb, char d)
{
	int8_t rc;

	if(RingbBytesFree(rb) == 0)
		return -1;

	//cli();
	rc = _RingbWrite(rb, d);
	//sei();
	return rc;
}
int8_t RingbWriteBuf(st_ringbuf *rb, uint8_t *buf, uint16_t anz)
{
	if(RingbBytesFree(rb) < anz)
	{
		return 1;
	}

	//cli();
	//NVIC_SETFAULTMASK();
	while(anz--)
		_RingbWrite(rb, *(buf++));
	//sei();
	return 0;
}

/**
// READ FROM RINGBUFFER
 *
 */
static int16_t inline _RingbRead(st_ringbuf *rb)
{
	uint8_t	data;

	data = rb->buf[(rb->rp)++];
	if(rb->rp >= rb->size)
		rb->rp = 0;
	(rb->cnt)--;
	return data;
}
int16_t RingbRead(st_ringbuf *rb)
{
	int16_t rc;

	if(RingbBytesInBuf(rb) == 0)
		return -1;

	//cli();
	rc = _RingbRead(rb);
	//sei();
	return rc;
}
int8_t RingbReadBuf(st_ringbuf *rb, uint8_t *buf, uint16_t anz)
{
	if(RingbBytesFree(rb) < anz)
	{
		return 1;
	}

	//cli();
	while(anz--)
		*(buf++) = _RingbRead(rb);
	//sei();
	return 0;
}


/** Private functions ---------------------------------------------------------*/
static void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);

  /* Enable USART clock */
  USARTx_CLK_INIT(USARTx_CLK, ENABLE);

  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

  /* Enable the USART OverSampling by 8 */
  //USART_OverSampling8Cmd(USARTx, ENABLE);

  /* USARTx configuration ----------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 5250000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 8
		     is: (USART APB Clock / 8)
			 Example:
			    - (USART3 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
			    - (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 16
		     is: (USART APB Clock / 16)
			 Example: (USART3 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
			 Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
        - Word Length = 8 Bits
        - one Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = BAUD;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);

  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable USART */
  USART_Cmd(USARTx, ENABLE);
}

void UartInit(void)
{
	/* USARTx configured as follow:
		- BaudRate = 115200 baud
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/
	RingbInit(rxRingb, sizeof(rxBuf));
	RingbInit(txRingb, sizeof(txBuf));

	USART_Config();
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
}

void uart_putc(char c)
{
/*	// write data to USART
	USARTx->DR = c;

	//Wait for the end of actual transmission
	while (!(USARTx->SR & USART_SR_TC)) {};   //Not this will be cleared only after a read from SR and then write to DR. (Therefore after this operation, it will still be set)
	*/
	//int	flg = (RingbBytesInBuf(txRingb) == 0);

	while(RingbBytesFree(txRingb) == 0)
		;
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
	RingbWrite(txRingb, (uint8_t)c);
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
}

/** Just for printf */
void uart_putchar(char c)
{
	if (c == '\n')
		uart_putc('\r');
	uart_putc(c);
}

/**
// send a string to USART
 *
 */
void uart_send(const char *text, int len)
{
	/*if(len > 0)
	{
		while(len > 1)
		{
			// write data to USART
			USARTx->DR = *(text++);

			//Wait for DR ready
			while (!(USARTx->SR & USART_SR_TXE)) {};
		}

		// write data to USART
		USARTx->DR = *(text++);

		//Wait for the end of actual transmission
		while (!(USARTx->SR & USART_SR_TC)) {};   //Not this will be cleared only after a read from SR and then write to DR. (Therefore after this operation, it will still be set)
	}*/
	while(len-- > 0)
		uart_putc(*(text++));
}

//
/// send a string to USART
//
void uart_puts(const char *text)
{
/*	char c;

	if(*text == 0)
		return;

	while(1)
	{
		c = *(text++);

		// write data to USART
		USARTx->DR = c;

		if(*text == 0)
			break;

		//Wait for DR ready
		while (!(USARTx->SR & USART_SR_TXE)) {};
	}

	//Wait for the end of actual transmission
	while (!(USARTx->SR & USART_SR_TC)) {};   //Not this will be cleared only after a read from SR and then write to DR. (Therefore after this operation, it will still be set)
*/
	while(*text != 0)
		uart_putc(*(text++));
}

//
/// send CR/LF to USART
//
void uart_putcrlf(void)
{
	uart_putc(13);
	uart_putc(10);
}

//
/// receive from USART
//
void uart_receive(char *buf, int cnt)
{
	while(cnt-- > 0)
	{
/*		//Wait for new data
		while (!(USARTx->SR & USART_SR_RXNE)) {};

		//Read the data
		*(buf++) = USARTx->DR;*/
		*(buf++) = uart_getch();
	}
}

//
/// receive from USART
//
int uart_getch()
{
	int16_t	data;

	while(RingbBytesInBuf(rxRingb) == 0)	;

	USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
	data = _RingbRead(rxRingb);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	return data;
}

//
/// test for USART byte received
//
int uart_rx()
{
	// test for new data
	//return (USARTx->SR & USART_SR_RXNE);
	return (RingbBytesInBuf(rxRingb) > 0);
}

//
/// get a string receiving from USART
//
char *uart_gets(char *buf, int size)
{
	//char	*s = buf;
	int		p = 0;
	char	c;

	while(1)
	{
		c = uart_getch();
		if(c == 13)
		{
			if(p +1 < size)
			{
				buf[p++] = 0;
			}
			uart_putchar(13);
			uart_putchar(10);
			break;
		}
		else if(c == 8)
		{
			if(p > 0 )
			{
				uart_putchar(c);
				uart_putchar(' ');
				uart_putchar(c);
				p--;
			}
		}
		else if((c & 0x7f) >= 32)
		{
			if(p +2 < size)
			{
				uart_putchar(c);
				buf[p++] = c;
			}
		}
	}
	return buf;
}

/******************************************************************************
*            STM32F4xx Peripherals Interrupt Handlers                        *
******************************************************************************/
/**
* @brief  This function handles USRAT interrupt request.
* @param  None
* @retval None
*/
void USARTx_IRQHandler(void)
{
	/* USART TX handling --------------------------------------------------*/
	if (USART_GetITStatus(USARTx, USART_IT_TXE) == SET)
	{
		if(RingbBytesInBuf(txRingb) == 0)
		{
			/* Disable the USARTx transmit data register empty interrupt */
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
			fTxDisabled = 1;
		}
		else
		{
			/* send data from ring buffer */
			USART_SendData(USARTx, _RingbRead(txRingb));
		}
	}

	/* USART RX handling --------------------------------------------------*/
	if (USART_GetITStatus(USARTx, USART_IT_RXNE) == SET)
	{
		uint16_t rcvBlc = USART_ReceiveData(USARTx);
		if(RingbBytesFree(rxRingb) > 0)
			RingbWrite(rxRingb, rcvBlc);
	}
}
