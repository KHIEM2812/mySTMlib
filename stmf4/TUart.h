#ifndef _tUart_h
#define _tUart_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "Uart.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TUartDMA();
void TUartDMAWithRunTimeString();
void TPrinf();
void TPrinfFloat();
void TMyPrintf();
void TMyPrintfWithFloat();
void TIsUsartTransferComplete();
void TUART_PutChar();
#endif
