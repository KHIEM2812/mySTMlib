/*
 This file is part of AutoQuad.

 AutoQuad is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 AutoQuad is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with AutoQuad.  If not, see <http://www.gnu.org/licenses/>.

 Copyright © 2011, 2012, 2013  Bill Nesbitt
 */

#ifndef _config_h
#define _config_h

#include "STM32_Nucleo/stm32f1xx/stm32f1xx_nucleo.h"
#include "IBox.h"
#include "flash.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#define CONFIG_FILE_NAME	    "params.txt"
#define CONFIG_FILE_BUF_SIZE	    512
#define CONFIG_LINE_BUF_SIZE	    128

#define CONFIG_NUM_PARAMS 		5

// param names must be kept <= 16 chars for mavlink
enum configParameters {
	CONFIG_VERSION = 0,
	NOWEIGHTLV,
	EMPTYWEIGHTLV,
	FULLWEIGHTLV,
	WEIGHTTOLLERANCE
};

typedef struct {
	unsigned int paramId;
	unsigned int num;
	float values[CONFIG_NUM_PARAMS];
}__attribute__((packed)) paramStruct_t;

extern float p[CONFIG_NUM_PARAMS];
extern const char *configParameterStrings[];

extern void ConfigInit(void);
extern void configFlashRead(void);
extern unsigned char configFlashWrite(void);

extern void configLoadDefault(void);
extern unsigned int configParameterRead(void *data);
extern unsigned int configParameterWrite(void *data);

extern void configSetParamByID(int id, float value);

#endif
