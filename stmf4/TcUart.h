#ifndef _tcUart_h
#define _tcUart_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "cUart.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TPrinf();
void TPrinfFloat();
void TIsUsartTransferComplete();
void TcUartMirroInputNOutput();

#endif
